// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskGoToDoor.generated.h"

/**
  * Searches for nearby maze Doors that match the bot's carried key color
 */
UCLASS(Meta = (DisplayName = "Go To Door C++"))
class MAZEGAME_API UBTTaskGoToDoor : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskGoToDoor();

protected:

	/* The search radius the bot will try to find a maze key */
	UPROPERTY(EditAnywhere)
	float SearchRadius;

	UPROPERTY(Transient)
	class AMazeGameBot* BotCache;

	UPROPERTY(Transient)
	class ABotController* BotConCache;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BBMazeDoor;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool AlreadyTracedActor(const TArray<AActor*>& TracedActors, const AActor& ActorToCheck) const;
};
