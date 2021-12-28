// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_GrabKey.h"
#include "MazeGame/AI/MazeGameBot.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "MazeGameKey.h"
#include "AI/BotController.h"
#include "DrawDebugHelpers.h"

UBTTask_GrabKey::UBTTask_GrabKey()
{
	SearchRadius = 600.f;
	BBMazeKey.SelectedKeyName = "MazeKey";
}


EBTNodeResult::Type UBTTask_GrabKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		DrawDebugSphere(GetWorld(), ((BotCache->GetActorLocation() + FVector(150.f) - BotCache->GetActorLocation()) / 2) + BotCache->GetActorLocation(), SearchRadius, 10, FColor::Green, true);

		//Perform a Sphere Sweep with the according radius to find a nearby maze key
		if (GetWorld()->SweepMultiByObjectType(OutHits, BotCache->GetActorLocation(), BotCache->GetActorLocation() + FVector(150.f), FQuat(), CollisionObjectParams, CollisionShape) && BotConCache && IsValid(BotConCache))
		{
			for (int32 Hit = 0; Hit < OutHits.Num(); Hit++)
			{
				if (OutHits[Hit].GetActor() && Cast<AMazeGameKey>(OutHits[Hit].GetActor()))
				{
					GLog->Log("found a maze key! updating location!");
					DrawDebugPoint(GetWorld(), OutHits[Hit].ImpactPoint, 25.f, FColor::Black, true);
					AMazeGameKey* Key = Cast<AMazeGameKey>(OutHits[Hit].GetActor());
					BlackboardComp->SetValueAsObject(BBMazeKey.SelectedKeyName, Key);

					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
		return EBTNodeResult::Failed;
}

