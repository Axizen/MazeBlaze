// AIErrorHandlingTestWidget.h
// Widget for running AI error handling tests and displaying results

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AIErrorHandlingTestWidget.generated.h"

/**
 * Widget for running AI error handling tests and displaying results
 */
UCLASS()
class MAZEBLAZE_API UAIErrorHandlingTestWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Run all AI error handling tests */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void RunAllTests();
    
    /** Run a specific AI error handling test */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void RunSpecificTest(const FString& TestName);
    
    /** Trigger a specific error on all AI controllers */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void TriggerErrorOnAllAI(const FString& ErrorType);
    
    /** Reset all AI controllers */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void ResetAllAI();
    
    /** Get the test results as a string */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    FString GetTestResults() const;
    
    /** Get the number of AI controllers in the level */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    int32 GetAIControllerCount() const;
    
    /** Get the number of AI controllers in error state */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    int32 GetAIControllersInErrorState() const;
    
    /** Get a list of all error states as strings */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    TArray<FString> GetAllErrorStates() const;
    
    /** Get a list of all error messages */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    TArray<FString> GetAllErrorMessages() const;
    
    /** Get the percentage of successful tests */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    float GetTestSuccessPercentage() const;
    
    /** Get the percentage of successful recoveries */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    float GetRecoverySuccessPercentage() const;
    
    /** Get the average recovery time */
    UFUNCTION(BlueprintPure, Category = "AI Testing")
    float GetAverageRecoveryTime() const;

private:
    /** The last test results */
    FString LastTestResults;
    
    /** The number of successful tests */
    int32 SuccessfulTests;
    
    /** The number of failed tests */
    int32 FailedTests;
    
    /** The number of recovery attempts */
    int32 RecoveryAttempts;
    
    /** The number of successful recoveries */
    int32 SuccessfulRecoveries;
    
    /** The average recovery time */
    float AverageRecoveryTime;
    
    /** Find all AI controllers in the level */
    TArray<class AMazeBlazeAIController*> GetAllAIControllers() const;
};