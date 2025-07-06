#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MazeBlazeKey.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazeAIController.h"
#include "BTTask_PickupKey.generated.h"

/**
 * Behavior Tree Task for picking up a key
 */
UCLASS()
class MAZEBLAZE_API UBTTask_PickupKey : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PickupKey();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the key to pick up
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector KeyToPickup;
};