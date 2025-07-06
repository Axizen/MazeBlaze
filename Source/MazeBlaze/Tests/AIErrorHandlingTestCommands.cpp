// AIErrorHandlingTestCommands.cpp
// Console commands for running AI error handling tests during gameplay

#include "AIErrorHandlingTestCommands.h"
#include "AIErrorHandlingTests.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

// Console command to run all AI error handling tests
static FAutoConsoleCommand RunAllAIErrorHandlingTestsCmd(
    TEXT("MazeBlaze.RunAllAIErrorTests"),
    TEXT("Runs all AI error handling tests"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunAllTests)
);

// Console commands to run individual tests
static FAutoConsoleCommand RunAssetMissingTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.AssetMissing"),
    TEXT("Tests handling of missing assets"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunAssetMissingTest)
);

static FAutoConsoleCommand RunBlackboardInitFailedTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.BlackboardInitFailed"),
    TEXT("Tests handling of blackboard initialization failure"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunBlackboardInitFailedTest)
);

static FAutoConsoleCommand RunBehaviorTreeStartFailedTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.BehaviorTreeStartFailed"),
    TEXT("Tests handling of behavior tree start failure"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunBehaviorTreeStartFailedTest)
);

static FAutoConsoleCommand RunNavigationMissingTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.NavigationMissing"),
    TEXT("Tests handling of navigation system errors"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunNavigationMissingTest)
);

static FAutoConsoleCommand RunPerceptionErrorTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.PerceptionError"),
    TEXT("Tests handling of perception system errors"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunPerceptionErrorTest)
);

static FAutoConsoleCommand RunTaskExecutionFailedTestCmd(
    TEXT("MazeBlaze.RunAIErrorTest.TaskExecutionFailed"),
    TEXT("Tests handling of task execution failures"),
    FConsoleCommandDelegate::CreateStatic(&FAIErrorHandlingTestCommands::RunTaskExecutionFailedTest)
);

// Implementation of console command functions
void FAIErrorHandlingTestCommands::RunAllTests()
{
    UWorld* World = GetGameWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot run AI error handling tests: No valid game world"));
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("Running all AI error handling tests..."));
    RunAIErrorHandlingTests(World);
}

void FAIErrorHandlingTestCommands::RunAssetMissingTest()
{
    RunSpecificTest(TEXT("AssetMissing"));
}

void FAIErrorHandlingTestCommands::RunBlackboardInitFailedTest()
{
    RunSpecificTest(TEXT("BlackboardInitFailed"));
}

void FAIErrorHandlingTestCommands::RunBehaviorTreeStartFailedTest()
{
    RunSpecificTest(TEXT("BehaviorTreeStartFailed"));
}

void FAIErrorHandlingTestCommands::RunNavigationMissingTest()
{
    RunSpecificTest(TEXT("NavigationMissing"));
}

void FAIErrorHandlingTestCommands::RunPerceptionErrorTest()
{
    RunSpecificTest(TEXT("PerceptionError"));
}

void FAIErrorHandlingTestCommands::RunTaskExecutionFailedTest()
{
    RunSpecificTest(TEXT("TaskExecutionFailed"));
}

void FAIErrorHandlingTestCommands::RunSpecificTest(const FString& TestName)
{
    UWorld* World = GetGameWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot run AI error handling test: No valid game world"));
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("Running AI error handling test: %s"), *TestName);
    
    FAIErrorHandlingTestFramework TestFramework(World);
    bool bTestPassed = TestFramework.RunTest(TestName);
    
    UE_LOG(LogTemp, Display, TEXT("Test %s: %s"), 
           *TestName, 
           bTestPassed ? TEXT("PASSED") : TEXT("FAILED"));
}

UWorld* FAIErrorHandlingTestCommands::GetGameWorld()
{
    UWorld* World = nullptr;
    
    // Try to find a game or PIE world
    for (const FWorldContext& Context : GEngine->GetWorldContexts())
    {
        if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
        {
            World = Context.World();
            break;
        }
    }
    
    return World;
}