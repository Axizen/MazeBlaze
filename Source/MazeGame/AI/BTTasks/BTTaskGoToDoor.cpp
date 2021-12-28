// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTaskGoToDoor.h"
#include "MazeGame/AI/MazeGameBot.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "MazeGameDoor.h"
#include "MazeGameKey.h"
#include "AI/BotController.h"
#include "DrawDebugHelpers.h"

UBTTaskGoToDoor::UBTTaskGoToDoor()
{
	SearchRadius = 600.f;
	BBMazeDoor.SelectedKeyName = "MazeDoor";
}


EBTNodeResult::Type UBTTaskGoToDoor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());
	BotCache = Cast<AMazeGameBot>(BotConCache->GetCharacter());
	UBlackboardComponent* BlackboardComp = BotConCache->GetBBComponent();

	if (BotCache && IsValid(BotCache) && BotConCache && IsValid(BotConCache))
	{
		FCollisionObjectQueryParams CollisionObjectParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic);

		FCollisionShape CollisionShape = FCollisionShape();
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(SearchRadius);

		TArray<FHitResult> OutHits;
		TArray<AActor*> MazeDoors;

		DrawDebugSphere(GetWorld(), ((BotCache->GetActorLocation() + FVector(150.f) - BotCache->GetActorLocation()) / 2) + BotCache->GetActorLocation(), SearchRadius, 10, FColor::Green, true);

		//Perform a sweep for nearby doors
		if (GetWorld()->SweepMultiByObjectType(OutHits, BotCache->GetActorLocation(), BotCache->GetActorLocation() + FVector(150.f), FQuat(), CollisionObjectParams, CollisionShape))
		{
			for (int32 HitIndex = 0; HitIndex < OutHits.Num(); HitIndex++)
			{
				AMazeGameDoor* HitDoor = Cast<AMazeGameDoor>(OutHits[HitIndex].GetActor());

				//Only include valid doors in our final contexts;
				//We want to exclude any duplicate actors
				if (HitDoor && IsValid(HitDoor) && !AlreadyTracedActor(MazeDoors, *HitDoor))
				{
					//MazeDoors.Add(HitDoor);

					if (BotCache->GetCarriedKey()->CanOpen(HitDoor))
					{
						BlackboardComp->SetValueAsObject(BBMazeDoor.SelectedKeyName,HitDoor);

						BotCache->WalkThroughDoor(HitDoor, BotConCache);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

bool UBTTaskGoToDoor::AlreadyTracedActor(const TArray<AActor*>& TracedActors, const AActor& ActorToCheck) const
{
	for (int32 ActorIndex = 0; ActorIndex < TracedActors.Num(); ActorIndex++)
	{
		if (TracedActors[ActorIndex]->GetName() == ActorToCheck.GetName())
		{
			//GLog->Log("already traced this actor!");
			return true;
		}
	}
	return false;
}

