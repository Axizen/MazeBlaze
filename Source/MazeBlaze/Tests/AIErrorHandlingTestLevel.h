// AIErrorHandlingTestLevel.h
// Helper class for setting up a test level for AI error handling tests

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIErrorHandlingTestLevel.generated.h"

/**
 * Actor that sets up a test environment for AI error handling tests
 * Place this in a level to create test scenarios for AI error handling
 */
UCLASS()
class MAZEBLAZE_API AAIErrorHandlingTestLevel : public AActor
{
    GENERATED_BODY()

public:
    AAIErrorHandlingTestLevel();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** Spawn AI characters for testing */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void SpawnTestAICharacters(int32 Count = 3);

    /** Create test zones for different error types */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void CreateTestZones();

    /** Trigger a specific error on all AI controllers */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void TriggerErrorOnAllAI(const FString& ErrorType);

    /** Reset all AI controllers to normal state */
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void ResetAllAI();

    /** Character class to spawn for testing */
    UPROPERTY(EditAnywhere, Category = "AI Testing")
    TSubclassOf<class AMazeBlazeAICharacter> AICharacterClass;

    /** Behavior tree to use for testing */
    UPROPERTY(EditAnywhere, Category = "AI Testing")
    class UBehaviorTree* TestBehaviorTree;

    /** Blackboard to use for testing */
    UPROPERTY(EditAnywhere, Category = "AI Testing")
    class UBlackboardData* TestBlackboard;

    /** Whether to automatically run tests on begin play */
    UPROPERTY(EditAnywhere, Category = "AI Testing")
    bool bAutoRunTestsOnBeginPlay;

    /** Delay before auto-running tests (seconds) */
    UPROPERTY(EditAnywhere, Category = "AI Testing", meta = (EditCondition = "bAutoRunTestsOnBeginPlay"))
    float AutoRunTestsDelay;

private:
    /** Array of spawned test AI characters */
    UPROPERTY()
    TArray<class AMazeBlazeAICharacter*> TestAICharacters;

    /** Array of spawned test zones */
    UPROPERTY()
    TArray<class AActor*> TestZones;

    /** Create a navigation error test zone */
    void CreateNavigationErrorZone(const FVector& Location);

    /** Create a task execution error test zone */
    void CreateTaskExecutionErrorZone(const FVector& Location);

    /** Create a perception error test zone */
    void CreatePerceptionErrorZone(const FVector& Location);

    /** Timer handle for auto-running tests */
    FTimerHandle AutoRunTestsTimerHandle;

    /** Run all tests automatically */
    void AutoRunTests();
};