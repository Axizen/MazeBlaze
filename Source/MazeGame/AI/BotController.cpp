// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BotController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MazeGameBot.h"
#include "MazeGameKey.h"
#include "MazeGameExit.h"
#include "Kismet/KismetMathLibrary.h"

ABotController::ABotController()
{

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}

void ABotController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BotCache = Cast<AMazeGameBot>(InPawn);

	if (!BTAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid BT Asset"));
		return;
	}

	if (BotCache && IsValid(BotCache))
	{
		BlackboardComp->InitializeBlackboard(*BlackboardComp->GetBlackboardAsset());
		BehaviorTreeComp->StartTree(*BTAsset);
	}
}

void ABotController::OnUnPossess()
{
	BotCache = nullptr;
	Super::OnUnPossess();
}

