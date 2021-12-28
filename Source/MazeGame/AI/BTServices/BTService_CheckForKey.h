// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_CheckForKey.generated.h"

class AMazeGameBot;
class ABotController;

/**
 * This service checks if the Bot needs a key.
 * If that's the case it updates the corresponding Blackboard Values
 */
UCLASS(Meta = (DisplayName = "Check For Key C++"))
class MAZEGAME_API UBTService_CheckForKey : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckForKey();

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BBPickupKey;

	UPROPERTY(Transient)
	AMazeGameBot* BotCache;

	UPROPERTY(Transient)
	ABotController* BotConCache;

	/*
	 * Executed every tick interval (which you can set from the Behavior Tree while have this particular service selected)
	 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
