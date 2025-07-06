// AIErrorHandlingTestRunner.cpp
// Blueprint-friendly class for running AI error handling tests

#include "AIErrorHandlingTestRunner.h"
#include "AIErrorHandlingTests.h"
#include "Engine/World.h"
#include "../MazeBlazeAIController.h"

bool UAIErrorHandlingTestRunner::RunAllAIErrorHandlingTests(const UObject* WorldContextObject)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot run AI error handling tests: Invalid world context"));
        return false;
    }
    
    int32 Result = RunAIErrorHandlingTests(World);
    return Result == 0;
}

bool UAIErrorHandlingTestRunner::RunSpecificAIErrorHandlingTest(const UObject* WorldContextObject, const FString& TestName)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot run AI error handling test: Invalid world context"));
        return false;
    }
    
    FAIErrorHandlingTestFramework TestFramework(World);
    return TestFramework.RunTest(TestName);
}

void UAIErrorHandlingTestRunner::TriggerErrorOnAIController(AMazeBlazeAIController* AIController, const FString& ErrorType)
{
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot trigger error: Invalid AI controller"));
        return;
    }
    
    // Determine which error type to trigger
    EAIErrorType ErrorTypeEnum = EAIErrorType::None;
    FString ErrorMessage = TEXT("Manually triggered error for testing");
    
    if (ErrorType.Equals(TEXT("AssetMissing"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::AssetMissing;
    }
    else if (ErrorType.Equals(TEXT("BlackboardInitFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::BlackboardInitFailed;
    }
    else if (ErrorType.Equals(TEXT("BehaviorTreeStartFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::BehaviorTreeStartFailed;
    }
    else if (ErrorType.Equals(TEXT("NavigationMissing"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::NavigationMissing;
    }
    else if (ErrorType.Equals(TEXT("PerceptionError"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::PerceptionError;
    }
    else if (ErrorType.Equals(TEXT("TaskExecutionFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::TaskExecutionFailed;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown error type: %s"), *ErrorType);
        return;
    }
    
    // Trigger the error
    AIController->ReportAIError(ErrorTypeEnum, ErrorMessage);
}

void UAIErrorHandlingTestRunner::ResetAIController(AMazeBlazeAIController* AIController)
{
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot reset AI controller: Invalid AI controller"));
        return;
    }
    
    // Get the controlled pawn
    APawn* ControlledPawn = AIController->GetPawn();
    if (ControlledPawn)
    {
        // Reset error state
        AIController->CurrentErrorState = EAIErrorType::None;
        AIController->LastErrorMessage = TEXT("");
        
        // Restart the AI controller
        AIController->UnPossess();
        AIController->Possess(ControlledPawn);
    }
}

FString UAIErrorHandlingTestRunner::GetAIControllerErrorState(AMazeBlazeAIController* AIController)
{
    if (!AIController)
    {
        return TEXT("Invalid Controller");
    }
    
    return UEnum::GetValueAsString(AIController->CurrentErrorState);
}

FString UAIErrorHandlingTestRunner::GetAIControllerLastErrorMessage(AMazeBlazeAIController* AIController)
{
    if (!AIController)
    {
        return TEXT("Invalid Controller");
    }
    
    return AIController->LastErrorMessage;
}

bool UAIErrorHandlingTestRunner::IsAIControllerInErrorState(AMazeBlazeAIController* AIController)
{
    if (!AIController)
    {
        return false;
    }
    
    return AIController->IsInErrorState();
}