// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGame/AI/MazeGameBot.h"
#include "BotController.h"
#include "MazeGameInteractable.h"
#include "MazeGameKey.h"
#include "MazeGameDoor.h"


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AMazeGameBot::AMazeGameBot()
{
}

void AMazeGameBot::BeginPlay()
{
	Super::BeginPlay();

}

void AMazeGameBot::WalkThroughDoor(AMazeGameDoor* OpenedDoor, ABotController* BotCon)
{
	if (OpenedDoor && IsValid(OpenedDoor) && BotCon && IsValid(BotCon))
	{
		auto InteractionPointALoc = OpenedDoor->GetInteractionPointA()->GetComponentLocation();
		auto InteractionPointBLoc = OpenedDoor->GetInteractionPointB()->GetComponentLocation();

		auto InteractionPointADist =  UKismetMathLibrary::Vector_Distance(this->GetActorLocation(), InteractionPointALoc);
		auto InteractionPointBDist = UKismetMathLibrary::Vector_Distance(this->GetActorLocation(), InteractionPointBLoc);

		if (InteractionPointADist < InteractionPointBDist)
		{
			BotCon->MoveToLocation(InteractionPointALoc, 5.0f, false, true, false, true);
			if (OpenedDoor->IsOpen())
			{
				BotCon->MoveToLocation(InteractionPointBLoc, 5.0f, false, true, false, true);
			}
		}
		else
		{
			BotCon->MoveToLocation(InteractionPointBLoc, 5.0f, false, true, false, true);
			if (OpenedDoor->IsOpen())
			{
				BotCon->MoveToLocation(InteractionPointALoc, 5.0f, false, true, false, true);
			}
		}
	}
}

void AMazeGameBot::BotPickupKey(AMazeGameKey* NewKey)
{
	if (!NewKey || !IsValid(NewKey) || !NewKey->Execute_CanInteractWith(NewKey, this))
	{
		return;
	}
	if (CarriedKey && IsValid(CarriedKey))
	{
		MazeKeyCollection.Add(NewKey);
		//CarriedKey->DropDownAt(NewKey->GetActorLocation());
		//LastKeyLocation = NewKey->GetActorLocation();
	}
	NewKey->PickUp();
	CarriedKey = NewKey;
	OnPickupKey.Broadcast(CarriedKey);
}

void AMazeGameBot::BotInteract()
{
	Interact();
}

