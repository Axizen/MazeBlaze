// AIErrorHandlingTestWidget.cpp
// Widget for running AI error handling tests and displaying results

#include "AIErrorHandlingTestWidget.h"
#include "AIErrorHandlingTestRunner.h"
#include "AIErrorHandlingTests.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "../MazeBlazeAIController.h"

void UAIErrorHandlingTestWidget::RunAllTests()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        LastTestResults = TEXT("Error: Invalid world context");
        return;
    }
    
    // Run all tests
    FAIErrorHandlingTestFramework TestFramework(World);
    bool bTestsPassed = TestFramework.RunAllTests();
    
    // Store the results
    const FAIErrorTestResults& Results = TestFramework.GetTestResults();
    LastTestResults = Results.GenerateReport();
    
    // Store the statistics
    SuccessfulTests = Results.SuccessfulTests;
    FailedTests = Results.FailedTests;
    RecoveryAttempts = Results.RecoveryAttempts;
    SuccessfulRecoveries = Results.SuccessfulRecoveries;
    AverageRecoveryTime = Results.AverageRecoveryTime;
}

void UAIErrorHandlingTestWidget::RunSpecificTest(const FString& TestName)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        LastTestResults = TEXT("Error: Invalid world context");
        return;
    }
    
    // Run the specific test
    FAIErrorHandlingTestFramework TestFramework(World);
    bool bTestPassed = TestFramework.RunTest(TestName);
    
    // Store the results
    const FAIErrorTestResults& Results = TestFramework.GetTestResults();
    LastTestResults = Results.GenerateReport();
    
    // Store the statistics
    SuccessfulTests = Results.SuccessfulTests;
    FailedTests = Results.FailedTests;
    RecoveryAttempts = Results.RecoveryAttempts;
    SuccessfulRecoveries = Results.SuccessfulRecoveries;
    AverageRecoveryTime = Results.AverageRecoveryTime;
}

void UAIErrorHandlingTestWidget::TriggerErrorOnAllAI(const FString& ErrorType)
{
    TArray<AMazeBlazeAIController*> AIControllers = GetAllAIControllers();
    
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController)
        {
            UAIErrorHandlingTestRunner::TriggerErrorOnAIController(AIController, ErrorType);
        }
    }
    
    LastTestResults = FString::Printf(TEXT("Triggered %s error on %d AI controllers"), *ErrorType, AIControllers.Num());
}

void UAIErrorHandlingTestWidget::ResetAllAI()
{
    TArray<AMazeBlazeAIController*> AIControllers = GetAllAIControllers();
    
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController)
        {
            UAIErrorHandlingTestRunner::ResetAIController(AIController);
        }
    }
    
    LastTestResults = FString::Printf(TEXT("Reset %d AI controllers"), AIControllers.Num());
}

FString UAIErrorHandlingTestWidget::GetTestResults() const
{
    return LastTestResults;
}

int32 UAIErrorHandlingTestWidget::GetAIControllerCount() const
{
    return GetAllAIControllers().Num();
}

int32 UAIErrorHandlingTestWidget::GetAIControllersInErrorState() const
{
    TArray<AMazeBlazeAIController*> AIControllers = GetAllAIControllers();
    
    int32 Count = 0;
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController && AIController->IsInErrorState())
        {
            Count++;
        }
    }
    
    return Count;
}

TArray<FString> UAIErrorHandlingTestWidget::GetAllErrorStates() const
{
    TArray<AMazeBlazeAIController*> AIControllers = GetAllAIControllers();
    TArray<FString> ErrorStates;
    
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController && AIController->IsInErrorState())
        {
            ErrorStates.Add(UAIErrorHandlingTestRunner::GetAIControllerErrorState(AIController));
        }
    }
    
    return ErrorStates;
}

TArray<FString> UAIErrorHandlingTestWidget::GetAllErrorMessages() const
{
    TArray<AMazeBlazeAIController*> AIControllers = GetAllAIControllers();
    TArray<FString> ErrorMessages;
    
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController && AIController->IsInErrorState())
        {
            ErrorMessages.Add(UAIErrorHandlingTestRunner::GetAIControllerLastErrorMessage(AIController));
        }
    }
    
    return ErrorMessages;
}

float UAIErrorHandlingTestWidget::GetTestSuccessPercentage() const
{
    int32 TotalTests = SuccessfulTests + FailedTests;
    return TotalTests > 0 ? (float)SuccessfulTests / TotalTests * 100.0f : 0.0f;
}

float UAIErrorHandlingTestWidget::GetRecoverySuccessPercentage() const
{
    return RecoveryAttempts > 0 ? (float)SuccessfulRecoveries / RecoveryAttempts * 100.0f : 0.0f;
}

float UAIErrorHandlingTestWidget::GetAverageRecoveryTime() const
{
    return AverageRecoveryTime;
}

TArray<AMazeBlazeAIController*> UAIErrorHandlingTestWidget::GetAllAIControllers() const
{
    TArray<AMazeBlazeAIController*> AIControllers;
    
    UWorld* World = GetWorld();
    if (!World)
    {
        return AIControllers;
    }
    
    for (TActorIterator<AMazeBlazeAIController> It(World); It; ++It)
    {
        AIControllers.Add(*It);
    }
    
    return AIControllers;
}