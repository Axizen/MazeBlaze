#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MazeBlazeExit.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazeAIController.h"
#include "BTTask_ReachExit.generated.h"

/**
 * Behavior Tree Task for reaching the maze exit
 */
UCLASS()
class MAZEBLAZE_API UBTTask_ReachExit : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ReachExit();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the exit location
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ExitLocation;
};