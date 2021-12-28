// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookForExit.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisplayName = "Look For Exit C++"))
class MAZEGAME_API UBTService_LookForExit : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_LookForExit();

private:

    UPROPERTY(Transient)
    class AMazeGameBot* BotCache;

    UPROPERTY(Transient)
    class ABotController* BotConCache;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BBFindExit;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
