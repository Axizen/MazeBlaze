// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/BTTask_FindExit.h"
#include "MazeGame/AI/MazeGameBot.h"
#include "AI/BotController.h"
#include "MazeGameExit.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h"

EBTNodeResult::Type UBTTask_FindExit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	BotCache = Cast<AMazeGameBot>(OwnerComp.GetOwner());

	if (BotCache && IsValid(BotCache))
	{
		FCollisionObjectQueryParams CollisionObjectParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic);

		FCollisionShape CollisionShape = FCollisionShape();
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(SearchRadius);

		TArray<FHitResult> OutHits;

		BotConCache = Cast<ABotController>(OwnerComp.GetAIOwner());

		DrawDebugSphere(GetWorld(), ((BotCache->GetActorLocation() + FVector(150.f) - BotCache->GetActorLocation()) / 2) + BotCache->GetActorLocation(), SearchRadius, 10, FColor::Green, true);

		//Perform a Sphere Sweep with the according radius to find the maze exit
		if (GetWorld()->SweepMultiByObjectType(OutHits, BotCache->GetActorLocation(), BotCache->GetActorLocation() + FVector(150.f), FQuat(), CollisionObjectParams, CollisionShape) && BotConCache && IsValid(BotConCache))
		{
			for (int32 Hit = 0; Hit < OutHits.Num(); Hit++)
			{
				if (OutHits[Hit].GetActor() && Cast<AMazeGameExit>(OutHits[Hit].GetActor()))
				{
					GLog->Log("found the maze exit! updating location!");
					DrawDebugPoint(GetWorld(), OutHits[Hit].ImpactPoint, 25.f, FColor::Black, true);
					AMazeGameExit* Exit = Cast<AMazeGameExit>(OutHits[Hit].GetActor());

					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}