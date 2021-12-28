// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Wander.generated.h"

/**
 * Wander behavior task for AI movement.
 */
UCLASS(Meta = (DisplayName = "Wander C++"))
class MAZEGAME_API UBTTask_Wander : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_Wander();

private:

    UPROPERTY(EditAnywhere, Category = "Wander")
    FVector WanderTarget;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderRadius;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderDistance;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderJitter;

    UPROPERTY(EditAnywhere, Category = "Wander")
    float WanderRatio;

    UPROPERTY(Transient)
    class AMazeGameBot* BotCache;

    UPROPERTY(Transient)
    class ABotController* BotConCache;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
