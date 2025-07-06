#include "BTTask_ReachExit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ReachExit::UBTTask_ReachExit()
{
	NodeName = TEXT("Reach Exit");
}

EBTNodeResult::Type UBTTask_ReachExit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	
	// Get the blackboard component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}
	
	// Get the exit location from the blackboard
	FVector ExitLoc = BlackboardComp->GetValueAsVector(ExitLocation.SelectedKeyName);
	if (ExitLoc.IsZero())
	{
		return EBTNodeResult::Failed;
	}
	
	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	// Find the nearest exit actor
	TArray<AActor*> FoundExits;
	UGameplayStatics::GetAllActorsOfClass(AIController->GetWorld(), AMazeBlazeExit::StaticClass(), FoundExits);
	
	AMazeBlazeExit* NearestExit = nullptr;
	float NearestDistance = MAX_FLT;
	
	for (AActor* Actor : FoundExits)
	{
		AMazeBlazeExit* Exit = Cast<AMazeBlazeExit>(Actor);
		if (Exit)
		{
			float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), Exit->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestExit = Exit;
			}
		}
	}
	
	if (!NearestExit)
	{
		return EBTNodeResult::Failed;
	}
	
	// Cast to MazeBlazeCharacter
	AMazeBlazeCharacter* Character = Cast<AMazeBlazeCharacter>(ControlledPawn);
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}
	
	// Check if we can interact with the exit
	if (!NearestExit->CanInteractWith_Implementation(Character))
	{
		return EBTNodeResult::Failed;
	}
	
	// Check if we're close enough to interact
	const float InteractionDistance = 200.0f; // Adjust as needed
	if (NearestDistance <= InteractionDistance)
	{
		// Interact with the exit
		NearestExit->InteractWith_Implementation(Character);
		return EBTNodeResult::Succeeded;
	}
	
	// Move to the exit
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIController->GetWorld());
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(NearestExit->GetActorLocation(), 100.0f, NavLocation))
	{
		AIController->MoveToLocation(NavLocation.Location);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

FString UBTTask_ReachExit::GetStaticDescription() const
{
	return FString::Printf(TEXT("Reach Exit: %s"), *ExitLocation.SelectedKeyName.ToString());
}