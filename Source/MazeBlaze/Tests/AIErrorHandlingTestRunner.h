// AIErrorHandlingTestRunner.h
// Blueprint-friendly class for running AI error handling tests

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AIErrorHandlingTestRunner.generated.h"

/**
 * Blueprint function library for running AI error handling tests
 */
UCLASS()
class MAZEBLAZE_API UAIErrorHandlingTestRunner : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Run all AI error handling tests */
    UFUNCTION(BlueprintCallable, Category = "AI Testing", meta = (WorldContext = "WorldContextObject"))
    static bool RunAllAIErrorHandlingTests(const UObject* WorldContextObject);
    
    /** Run a specific AI error handling test */
    UFUNCTION(BlueprintCallable, Category = "AI Testing", meta = (WorldContext = "WorldContextObject"))
    static bool RunSpecificAIErrorHandlingTest(const UObject* WorldContextObject, const FString& TestName);
    
    /** Trigger a specific error on an AI controller */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    static void TriggerErrorOnAIController(class AMazeBlazeAIController* AIController, const FString& ErrorType);
    
    /** Reset an AI controller to normal state */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    static void ResetAIController(class AMazeBlazeAIController* AIController);
    
    /** Get the current error state of an AI controller as a string */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    static FString GetAIControllerErrorState(class AMazeBlazeAIController* AIController);
    
    /** Get the last error message of an AI controller */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    static FString GetAIControllerLastErrorMessage(class AMazeBlazeAIController* AIController);
    
    /** Check if an AI controller is in an error state */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    static bool IsAIControllerInErrorState(class AMazeBlazeAIController* AIController);
};