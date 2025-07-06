// BTService_ErrorDetection.h
// Behavior Tree service that monitors for AI errors

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MazeBlazeAIController.h"
#include "BTService_ErrorDetection.generated.h"

/**
 * Behavior Tree service that continuously monitors for AI errors
 * This service should be attached to the root node of the behavior tree
 */
UCLASS()
class MAZEBLAZE_API UBTService_ErrorDetection : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_ErrorDetection();
    
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    virtual FString GetStaticDescription() const override;
    
 protected:
    // Maximum time to wait for path following to succeed
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxPathFollowingTime = 10.0f;
    
    // Maximum time to wait for a task to complete
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxTaskExecutionTime = 15.0f;
    
    // Maximum time allowed to be stuck in one place
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxStuckTime = 5.0f;
    
    // Distance threshold for considering AI stuck
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float StuckDistanceThreshold = 50.0f;
    
    // Whether to draw debug information
    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bDrawDebugInfo = true;
    
private:
    // Time tracking for path following
    float PathFollowingTimer = 0.0f;
    
    // Last known location for stuck detection
    FVector LastLocation = FVector::ZeroVector;
    
    // Time at same location
    float StuckTime = 0.0f;
    
    // Last known state
    EAIState LastState;
    
    // Time in current state
    float TimeInState = 0.0f;
    
    // Check if AI is stuck
    bool IsAIStuck(const AAIController* AIController, const FVector& CurrentLocation);
    
    // Check for navigation errors
    void CheckNavigationErrors(AMazeBlazeAIController* MazeAIController, const FVector& CurrentLocation);
    
    // Check for state-specific errors
    void CheckStateErrors(AMazeBlazeAIController* MazeAIController, UBlackboardComponent* BlackboardComp);
};