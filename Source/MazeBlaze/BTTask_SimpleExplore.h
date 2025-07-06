#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MazeBlazeAIController.h"
#include "BTTask_SimpleExplore.generated.h"

/**
 * Behavior Tree Task for simple exploration of the maze
 */
UCLASS()
class MAZEBLAZE_API UBTTask_SimpleExplore : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SimpleExplore();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key to store the exploration target
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ExplorationTarget;

	// Maximum exploration distance
	UPROPERTY(EditAnywhere, Category = "Exploration", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
	float MaxExplorationDistance = 1000.0f;
};