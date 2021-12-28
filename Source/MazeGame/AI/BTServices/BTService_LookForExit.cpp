// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_LookForExit.h"
#include "AI/MazeGameBot.h"
#include "AI/BotController.h"
#include "MazeGameExit.h"

UBTService_LookForExit::UBTService_LookForExit()
{
	BBFindExit.SelectedKeyName = "LookingForExit";
}

void UBTService_LookForExit::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());
	BotCache = Cast<AMazeGameBot>(BotConCache->GetCharacter());
	UBlackboardComponent* BlackboardComp = BotConCache->GetBBComponent();

	if (BotConCache && IsValid(BotConCache) && BotCache && IsValid(BotCache))
	{
		//GLog->Log("Find the exit!");
		BlackboardComp->SetValueAsBool(BBFindExit.SelectedKeyName, true);
	}
}
