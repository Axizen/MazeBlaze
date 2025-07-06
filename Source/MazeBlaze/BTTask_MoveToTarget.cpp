#include "BTTask_MoveToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "MazeBlazeAIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	NodeName = TEXT("Move To Target");
	
	// Accept vectors as the blackboard key
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToTarget, BlackboardKey));
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	// Get the target location from the blackboard
	FVector TargetLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
	if (TargetLocation.IsZero())
	{
		return EBTNodeResult::Failed;
	}
	
	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	// Check if we're already at the target location
	const float DistanceToTarget = FVector::Dist(ControlledPawn->GetActorLocation(), TargetLocation);
	if (DistanceToTarget <= AcceptableRadius)
	{
		return EBTNodeResult::Succeeded;
	}
	
	// Move to the target location
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(TargetLocation);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);
	MoveRequest.SetUsePathfinding(bUsePathfinding);
	MoveRequest.SetAllowPartialPath(bAllowPartialPath);
	MoveRequest.SetProjectGoalLocation(bProjectGoalLocation);
	
	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest, &NavPath);
	
	// Check if we have a valid path
	if (!NavPath || (NavPath->IsPartial() && !bAllowPartialPath))
	{
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && TaskResult != EBTNodeResult::InProgress)
	{
		// Stop movement when the task is finished
		AIController->StopMovement();
	}
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

FString UBTTask_MoveToTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Move To Target: %s\nAcceptable Radius: %.1f"), *BlackboardKey.SelectedKeyName.ToString(), AcceptableRadius);
}