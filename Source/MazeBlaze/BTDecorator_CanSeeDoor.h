#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MazeGameDoor.h"
#include "BTDecorator_CanSeeDoor.generated.h"

/**
 * Behavior Tree Decorator that checks if the AI can see a door
 */
UCLASS()
class MAZEBLAZE_API UBTDecorator_CanSeeDoor : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanSeeDoor();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the visible door
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector VisibleDoorKey;
};