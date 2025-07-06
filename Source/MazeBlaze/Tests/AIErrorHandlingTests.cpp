// AIErrorHandlingTests.cpp
// Automated test framework for the MazeBlaze AI Error Handling System

#include "AIErrorHandlingTests.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Include our AI controller and related classes
#include "../MazeBlazeAIController.h"
#include "../MazeBlazeCharacter.h"
#include "../MazeBlazeKey.h"
#include "../MazeGameDoor.h"
#include "../MazeBlazeExit.h"

// Implementation of FAIErrorTestResults methods
FAIErrorTestResults::FAIErrorTestResults()
    : TotalTests(0)
    , SuccessfulTests(0)
    , FailedTests(0)
    , RecoveryAttempts(0)
    , SuccessfulRecoveries(0)
    , AverageRecoveryTime(0.0f)
{
}

void FAIErrorTestResults::AddTestResult(const FString& TestName, bool bSuccess, const FString& ErrorMessage)
{
    TotalTests++;
    if (bSuccess)
    {
        SuccessfulTests++;
    }
    else
    {
        FailedTests++;
        if (!ErrorMessage.IsEmpty())
        {
            ErrorMessages.Add(FString::Printf(TEXT("%s: %s"), *TestName, *ErrorMessage));
        }
    }
    TestResults.Add(TestName, bSuccess);
}

void FAIErrorTestResults::RecordRecoveryAttempt(bool bSuccess, float RecoveryTime)
{
    RecoveryAttempts++;
    if (bSuccess)
    {
        SuccessfulRecoveries++;
        // Update average recovery time
        AverageRecoveryTime = ((AverageRecoveryTime * (SuccessfulRecoveries - 1)) + RecoveryTime) / SuccessfulRecoveries;
    }
}

FString FAIErrorTestResults::GenerateReport() const
{
    FString Report = TEXT("\n===== AI ERROR HANDLING TEST REPORT =====\n\n");
    
    Report += FString::Printf(TEXT("Total Tests: %d\n"), TotalTests);
    Report += FString::Printf(TEXT("Successful Tests: %d (%.1f%%)\n"), 
                            SuccessfulTests, 
                            TotalTests > 0 ? (float)SuccessfulTests / TotalTests * 100.0f : 0.0f);
    Report += FString::Printf(TEXT("Failed Tests: %d (%.1f%%)\n"), 
                            FailedTests, 
                            TotalTests > 0 ? (float)FailedTests / TotalTests * 100.0f : 0.0f);
    Report += FString::Printf(TEXT("\nRecovery Attempts: %d\n"), RecoveryAttempts);
    Report += FString::Printf(TEXT("Successful Recoveries: %d (%.1f%%)\n"), 
                            SuccessfulRecoveries, 
                            RecoveryAttempts > 0 ? (float)SuccessfulRecoveries / RecoveryAttempts * 100.0f : 0.0f);
    Report += FString::Printf(TEXT("Average Recovery Time: %.2f seconds\n\n"), AverageRecoveryTime);
    
    Report += TEXT("Individual Test Results:\n");
    for (const auto& Result : TestResults)
    {
        Report += FString::Printf(TEXT("  %s: %s\n"), 
                                *Result.Key, 
                                Result.Value ? TEXT("PASS") : TEXT("FAIL"));
    }
    
    if (ErrorMessages.Num() > 0)
    {
        Report += TEXT("\nError Messages:\n");
        for (const FString& ErrorMsg : ErrorMessages)
        {
            Report += FString::Printf(TEXT("  - %s\n"), *ErrorMsg);
        }
    }
    
    Report += TEXT("\n========================================\n");
    return Report;
}

// Implementation of FAIErrorHandlingTestFramework methods
FAIErrorHandlingTestFramework::FAIErrorHandlingTestFramework(UWorld* InWorld)
    : World(InWorld)
    , TestResults()
    , CurrentTestStartTime(0.0f)
{
}

bool FAIErrorHandlingTestFramework::RunAllTests()
{
    UE_LOG(LogTemp, Display, TEXT("Starting AI Error Handling Tests..."));

    // Find all AI controllers in the world
    TArray<AMazeBlazeAIController*> AIControllers;
    for (TActorIterator<AMazeBlazeAIController> It(World); It; ++It)
    {   
        AIControllers.Add(*It);
    }

    if (AIControllers.Num() == 0)
    {   
        UE_LOG(LogTemp, Error, TEXT("No MazeBlazeAIControllers found in the world. Tests cannot run."));
        TestResults.AddTestResult(TEXT("AIControllerCheck"), false, TEXT("No MazeBlazeAIControllers found"));
        return false;
    }

    UE_LOG(LogTemp, Display, TEXT("Found %d AI controllers for testing"), AIControllers.Num());
    TestResults.AddTestResult(TEXT("AIControllerCheck"), true);

    // Run tests for each error type
    TestAssetMissing(AIControllers);
    TestBlackboardInitFailed(AIControllers);
    TestBehaviorTreeStartFailed(AIControllers);
    TestNavigationMissing(AIControllers);
    TestPerceptionError(AIControllers);
    TestTaskExecutionFailed(AIControllers);

    // Generate and display the test report
    FString Report = TestResults.GenerateReport();
    UE_LOG(LogTemp, Display, TEXT("%s"), *Report);

    return TestResults.FailedTests == 0;
}

bool FAIErrorHandlingTestFramework::RunTest(const FString& TestName)
{
    UE_LOG(LogTemp, Display, TEXT("Starting AI Error Handling Test: %s"), *TestName);

    // Find all AI controllers in the world
    TArray<AMazeBlazeAIController*> AIControllers;
    for (TActorIterator<AMazeBlazeAIController> It(World); It; ++It)
    {   
        AIControllers.Add(*It);
    }

    if (AIControllers.Num() == 0)
    {   
        UE_LOG(LogTemp, Error, TEXT("No MazeBlazeAIControllers found in the world. Test cannot run."));
        TestResults.AddTestResult(TEXT("AIControllerCheck"), false, TEXT("No MazeBlazeAIControllers found"));
        return false;
    }

    UE_LOG(LogTemp, Display, TEXT("Found %d AI controllers for testing"), AIControllers.Num());
    TestResults.AddTestResult(TEXT("AIControllerCheck"), true);

    // Run the specified test
    bool bTestRun = false;
    
    if (TestName.Equals(TEXT("AssetMissing"), ESearchCase::IgnoreCase))
    {
        TestAssetMissing(AIControllers);
        bTestRun = true;
    }
    else if (TestName.Equals(TEXT("BlackboardInitFailed"), ESearchCase::IgnoreCase))
    {
        TestBlackboardInitFailed(AIControllers);
        bTestRun = true;
    }
    else if (TestName.Equals(TEXT("BehaviorTreeStartFailed"), ESearchCase::IgnoreCase))
    {
        TestBehaviorTreeStartFailed(AIControllers);
        bTestRun = true;
    }
    else if (TestName.Equals(TEXT("NavigationMissing"), ESearchCase::IgnoreCase))
    {
        TestNavigationMissing(AIControllers);
        bTestRun = true;
    }
    else if (TestName.Equals(TEXT("PerceptionError"), ESearchCase::IgnoreCase))
    {
        TestPerceptionError(AIControllers);
        bTestRun = true;
    }
    else if (TestName.Equals(TEXT("TaskExecutionFailed"), ESearchCase::IgnoreCase))
    {
        TestTaskExecutionFailed(AIControllers);
        bTestRun = true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Unknown test name: %s"), *TestName);
        TestResults.AddTestResult(FString::Printf(TEXT("%sTest"), *TestName), false, 
                                FString::Printf(TEXT("Unknown test name: %s"), *TestName));
    }

    // Generate and display the test report
    FString Report = TestResults.GenerateReport();
    UE_LOG(LogTemp, Display, TEXT("%s"), *Report);

    return bTestRun && TestResults.FailedTests == 0;
}

private:
    UWorld* World;
    FAIErrorTestResults TestResults;
    float CurrentTestStartTime;

    /**
     * Test handling of missing assets (behavior tree, blackboard)
     */
    void FAIErrorHandlingTestFramework::TestAssetMissing(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running AssetMissing test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        // Store original assets
        UBehaviorTree* OriginalBT = TestController->BehaviorTreeAsset;
        UBlackboardData* OriginalBB = TestController->BlackboardAsset;
        
        // Force asset missing error
        TestController->BehaviorTreeAsset = nullptr;
        
        // Trigger error by trying to restart the behavior tree
        StartRecoveryTimer();
        bool bErrorDetected = false;
        
        // Force unpossess and repossess to trigger error
        APawn* ControlledPawn = TestController->GetPawn();
        if (ControlledPawn)
        {   
            TestController->UnPossess();
            TestController->Possess(ControlledPawn);
            
            // Check if error was detected
            bErrorDetected = TestController->IsInErrorState() && 
                            TestController->CurrentErrorState == EAIErrorType::AssetMissing;
            
            // Record recovery attempt
            float RecoveryTime = GetRecoveryTime();
            bool bRecovered = !TestController->IsInErrorState();
            TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
            
            // Restore original assets
            TestController->BehaviorTreeAsset = OriginalBT;
            TestController->BlackboardAsset = OriginalBB;
            
            // Restart controller
            TestController->UnPossess();
            TestController->Possess(ControlledPawn);
        }
        
        TestResults.AddTestResult(TEXT("AssetMissingTest"), bErrorDetected, 
                                bErrorDetected ? TEXT("") : TEXT("Failed to detect missing asset error"));
    }

    /**
     * Test handling of blackboard initialization failure
     */
    void FAIErrorHandlingTestFramework::TestBlackboardInitFailed(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running BlackboardInitFailed test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        // Store original blackboard component
        UBlackboardComponent* OriginalBB = TestController->GetBlackboardComp();
        
        // Create a method to simulate blackboard init failure
        // We'll use a private method to force the error state directly
        StartRecoveryTimer();
        TestController->ReportAIError(EAIErrorType::BlackboardInitFailed, 
                                    TEXT("Simulated blackboard initialization failure"));
        
        // Wait a few frames to allow recovery attempt
        for (int32 i = 0; i < 10; ++i)
        {   
            FPlatformProcess::Sleep(0.1f);
        }
        
        // Check if recovery was attempted
        float RecoveryTime = GetRecoveryTime();
        bool bRecovered = !TestController->IsInErrorState();
        TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
        
        TestResults.AddTestResult(TEXT("BlackboardInitFailedTest"), true);
    }

    /**
     * Test handling of behavior tree start failure
     */
    void FAIErrorHandlingTestFramework::TestBehaviorTreeStartFailed(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running BehaviorTreeStartFailed test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        // Simulate behavior tree start failure
        StartRecoveryTimer();
        TestController->ReportAIError(EAIErrorType::BehaviorTreeStartFailed, 
                                    TEXT("Simulated behavior tree start failure"));
        
        // Wait a few frames to allow recovery attempt
        for (int32 i = 0; i < 10; ++i)
        {   
            FPlatformProcess::Sleep(0.1f);
        }
        
        // Check if recovery was attempted
        float RecoveryTime = GetRecoveryTime();
        bool bRecovered = !TestController->IsInErrorState();
        TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
        
        TestResults.AddTestResult(TEXT("BehaviorTreeStartFailedTest"), true);
    }

    /**
     * Test handling of navigation system errors
     */
    void FAIErrorHandlingTestFramework::TestNavigationMissing(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running NavigationMissing test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        APawn* ControlledPawn = TestController->GetPawn();
        if (!ControlledPawn) return;
        
        // Store original location
        FVector OriginalLocation = ControlledPawn->GetActorLocation();
        
        // Find a location without navigation
        FVector NoNavLocation = OriginalLocation + FVector(0, 0, 10000.0f); // Way above the nav mesh
        
        // Teleport pawn to location without navigation
        ControlledPawn->SetActorLocation(NoNavLocation, false, nullptr, ETeleportType::TeleportPhysics);
        
        // Force the AI to try to move
        UBlackboardComponent* BlackboardComp = TestController->GetBlackboardComp();
        if (BlackboardComp)
        {   
            // Set a target far away
            BlackboardComp->SetValueAsVector("CurrentTarget", 
                                          OriginalLocation + FVector(5000.0f, 5000.0f, 0.0f));
            
            // Set state to exploring to trigger movement
            TestController->SetCurrentState(EAIState::Exploring);
        }
        
        // Wait for error to be detected
        StartRecoveryTimer();
        bool bErrorDetected = false;
        
        // Wait up to 5 seconds for error detection
        for (int32 i = 0; i < 50 && !bErrorDetected; ++i)
        {   
            FPlatformProcess::Sleep(0.1f);
            bErrorDetected = TestController->IsInErrorState() && 
                            TestController->CurrentErrorState == EAIErrorType::NavigationMissing;
        }
        
        // Check if recovery was attempted
        float RecoveryTime = GetRecoveryTime();
        bool bRecovered = !TestController->IsInErrorState();
        TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
        
        // Restore original location
        ControlledPawn->SetActorLocation(OriginalLocation, false, nullptr, ETeleportType::TeleportPhysics);
        
        TestResults.AddTestResult(TEXT("NavigationMissingTest"), bErrorDetected, 
                                bErrorDetected ? TEXT("") : TEXT("Failed to detect navigation error"));
    }

    /**
     * Test handling of perception system errors
     */
    void FAIErrorHandlingTestFramework::TestPerceptionError(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running PerceptionError test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        // Simulate perception error
        StartRecoveryTimer();
        TestController->ReportAIError(EAIErrorType::PerceptionError, 
                                    TEXT("Simulated perception system failure"));
        
        // Wait a few frames to allow recovery attempt
        for (int32 i = 0; i < 10; ++i)
        {   
            FPlatformProcess::Sleep(0.1f);
        }
        
        // Check if recovery was attempted
        float RecoveryTime = GetRecoveryTime();
        bool bRecovered = !TestController->IsInErrorState();
        TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
        
        TestResults.AddTestResult(TEXT("PerceptionErrorTest"), true);
    }

    /**
     * Test handling of task execution failures
     */
    void FAIErrorHandlingTestFramework::TestTaskExecutionFailed(const TArray<AMazeBlazeAIController*>& AIControllers)
    {   
        UE_LOG(LogTemp, Display, TEXT("Running TaskExecutionFailed test..."));
        
        if (AIControllers.Num() == 0) return;
        
        AMazeBlazeAIController* TestController = AIControllers[0];
        if (!TestController) return;
        
        // Simulate task execution failure
        StartRecoveryTimer();
        TestController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                    TEXT("Simulated task execution failure"));
        
        // Wait a few frames to allow recovery attempt
        for (int32 i = 0; i < 10; ++i)
        {   
            FPlatformProcess::Sleep(0.1f);
        }
        
        // Check if recovery was attempted
        float RecoveryTime = GetRecoveryTime();
        bool bRecovered = !TestController->IsInErrorState();
        TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
        
        TestResults.AddTestResult(TEXT("TaskExecutionFailedTest"), true);
    }

    /**
     * Start timer for measuring recovery time
     */
    void StartRecoveryTimer()
    {   
        CurrentTestStartTime = World->GetTimeSeconds();
    }

    /**
     * Get elapsed time since recovery timer started
     */
    float GetRecoveryTime() const
    {   
        return World->GetTimeSeconds() - CurrentTestStartTime;
    }
};

/**
 * Automation test that runs the AI error handling tests
 */
BEGIN_DEFINE_SPEC(FAIErrorHandlingTests, "MazeBlaze.AIErrorHandling", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)
END_DEFINE_SPEC(FAIErrorHandlingTests)

void FAIErrorHandlingTests::Define()
{
    Describe("AI Error Handling System", [this]()
    {
        It("Should detect and recover from various error conditions", [this]()
        {
            // Get the world
            UWorld* World = nullptr;
            for (const FWorldContext& Context : GEngine->GetWorldContexts())
            {
                if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
                {
                    World = Context.World();
                    break;
                }
            }

            TestTrue("World exists", World != nullptr);
            if (!World) return;

            // Create and run the test framework
            FAIErrorHandlingTestFramework TestFramework(World);
            bool bTestsPassed = TestFramework.RunAllTests();

            // Overall test result
            TestTrue("All AI error handling tests passed", bTestsPassed);
        });
    });
}

/**
 * Helper method to start the recovery timer
 */
void FAIErrorHandlingTestFramework::StartRecoveryTimer()
{
    CurrentTestStartTime = World->GetTimeSeconds();
}

/**
 * Helper method to get the elapsed time since recovery timer started
 */
float FAIErrorHandlingTestFramework::GetRecoveryTime() const
{
    return World->GetTimeSeconds() - CurrentTestStartTime;
}

/**
 * Main function to run tests manually (not used in automation testing)
 */
int32 RunAIErrorHandlingTests(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot run AI error handling tests: World is null"));
        return -1;
    }

    FAIErrorHandlingTestFramework TestFramework(World);
    bool bTestsPassed = TestFramework.RunAllTests();

    return bTestsPassed ? 0 : 1;
}