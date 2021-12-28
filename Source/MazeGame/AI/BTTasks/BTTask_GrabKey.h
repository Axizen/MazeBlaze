// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GrabKey.generated.h"

/**
 * Searches for nearby Maze Keys in the area and updates the corresponding Blackboard values
 */
UCLASS(Meta = (DisplayName = "Grab Key C++"))
class MAZEGAME_API UBTTask_GrabKey : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GrabKey();

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
	FBlackboardKeySelector BBMazeKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
