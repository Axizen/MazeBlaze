// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookForDoor.generated.h"

class AMazeGameBot;
class ABotController;

/**
 * Checks for any closed doors.
 */
UCLASS(Meta = (DisplayName = "Look For Door C++"))
class MAZEGAME_API UBTService_LookForDoor : public UBTService
{
	GENERATED_BODY()
public:
    UBTService_LookForDoor();

protected:

    UPROPERTY(Transient)
    AMazeGameBot* BotCache;

    UPROPERTY(Transient)
    ABotController* BotConCache;

private:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBKeyTargetMazeDoor;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBLookForLockedDoor;

    UPROPERTY(EditAnywhere, Category = "Node")
    float Distance;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
