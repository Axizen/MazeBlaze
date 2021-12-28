// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/BTService_LookForDoor.h"
#include "AI/MazeGameBot.h"
#include "AI/BotController.h"
#include "MazeGameDoor.h"
#include "MazeGameKey.h"
#include "Math/Vector.h"


UBTService_LookForDoor::UBTService_LookForDoor()
{
	BBKeyTargetMazeDoor.SelectedKeyName = "MazeDoor";
	BBLookForLockedDoor.SelectedKeyName = "LookingForDoor";

}

void UBTService_LookForDoor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());

	UBlackboardComponent* BlackboardComp = BotConCache->GetBBComponent();

	if (BotConCache && IsValid(BotConCache) && BotCache && IsValid(BotCache))
	{
		UObject* TargetDoor = BlackboardComp->GetValueAsObject(BBKeyTargetMazeDoor.SelectedKeyName);
		AMazeGameDoor* MazeDoor = Cast<AMazeGameDoor>(TargetDoor);

		if (MazeDoor && IsValid(MazeDoor))
		{
			BlackboardComp->SetValueAsBool(BBLookForLockedDoor.SelectedKeyName, !MazeDoor->IsOpen());
		}
	}

}