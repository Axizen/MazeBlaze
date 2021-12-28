// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindExit.generated.h"

/**
 * Searches for nearby Maze Exit in the area and updates the corresponding Blackboard values
 */
UCLASS(Meta = (DisplayName = "Find Exit C++"))
class MAZEGAME_API UBTTask_FindExit : public UBTTaskNode
{
	GENERATED_BODY()
	

	/*
	* Executes the task when we have reached the node
	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	/* The search radius the bot will try to find the maze exit */
	UPROPERTY(EditAnywhere)
	float SearchRadius = 600.f;

	UPROPERTY(Transient)
	class AMazeGameBot* BotCache;

	UPROPERTY(Transient)
	class ABotController* BotConCache;
};
