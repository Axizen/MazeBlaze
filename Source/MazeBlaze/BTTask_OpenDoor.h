#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MazeGameDoor.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazeAIController.h"
#include "BTTask_OpenDoor.generated.h"

/**
 * Behavior Tree Task for opening a door with a key
 */
UCLASS()
class MAZEBLAZE_API UBTTask_OpenDoor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_OpenDoor();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the door to open
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DoorToOpen;
};