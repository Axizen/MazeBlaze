#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToTarget.generated.h"

/**
 * Behavior Tree Task for moving to a target location
 */
UCLASS()
class MAZEBLAZE_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_MoveToTarget();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual FString GetStaticDescription() const override;

	// How close the AI needs to get to the target
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
	float AcceptableRadius = 50.0f;

	// Whether to use pathfinding
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUsePathfinding = true;

	// Whether to allow partial paths
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bAllowPartialPath = true;

	// Whether to project the target point to navigation
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bProjectGoalLocation = true;
};