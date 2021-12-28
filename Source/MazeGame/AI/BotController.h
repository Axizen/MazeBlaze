// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MazeGameBot.h"
#include "AIController.h"
#include "../Classes/BehaviorTree/BlackboardComponent.h"
#include "../Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BotController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MAZEGAME_API ABotController : public AAIController
{
	GENERATED_BODY()

public:
	ABotController();

	FORCEINLINE UBlackboardComponent* GetBBComponent() const { return BlackboardComp; }
	FORCEINLINE UBehaviorTreeComponent* GetBTComponent() const { return BehaviorTreeComp; }
	FORCEINLINE UBehaviorTree* GetCurrentTree() { return BTAsset; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BTAsset;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:

	UPROPERTY(Transient)
	AMazeGameBot* BotCache;

	UPROPERTY(EditAnywhere, Category = "AI")
    class UBlackboardComponent* BlackboardComp;

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTreeComponent* BehaviorTreeComp;
};
