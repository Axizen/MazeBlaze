// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_CheckForKey.h"
#include "AI/MazeGameBot.h"
#include "AI/BotController.h"
#include "MazeGameDoor.h"


UBTService_CheckForKey::UBTService_CheckForKey()
{
	BBPickupKey.SelectedKeyName = "PickupKey";
}

void UBTService_CheckForKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());
	BotCache = Cast<AMazeGameBot>(BotConCache->GetCharacter());
	UBlackboardComponent* BlackboardComp = BotConCache->GetBlackboardComponent();

	if (BotConCache && IsValid(BotConCache) && BotCache && IsValid(BotCache))
	{
		/*if (!BotCache->IsCarryingKey())
		{
			GLog->Log("I need a key!");

		}
		else if (BotCache->IsCarryingKey())
		{
			GLog->Log("I need another key!");
		}
		GLog->Log("setting door needs key");
		*/
		BlackboardComp->SetValueAsBool(BBPickupKey.SelectedKeyName, true);
		
	}
}