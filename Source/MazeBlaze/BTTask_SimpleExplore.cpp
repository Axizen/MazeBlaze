#include "BTTask_SimpleExplore.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "MazeBlazeAIController.h"
#include "DrawDebugHelpers.h"

UBTTask_SimpleExplore::UBTTask_SimpleExplore()
{
	NodeName = TEXT("Simple Explore");
}

EBTNodeResult::Type UBTTask_SimpleExplore::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("SimpleExplore: No AI controller!"));
		return EBTNodeResult::Failed;
	}
	
	// Check if we have our custom AI controller for error reporting
	AMazeBlazeAIController* MazeAIController = Cast<AMazeBlazeAIController>(AIController);
	
	// Get the blackboard component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Error, TEXT("SimpleExplore: No blackboard component!"));
		if (MazeAIController)
		{
			MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
				TEXT("SimpleExplore: Missing blackboard component"));
		}
		return EBTNodeResult::Failed;
	}
	
	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("SimpleExplore: No controlled pawn!"));
		if (MazeAIController)
		{
			MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
				TEXT("SimpleExplore: No controlled pawn"));
		}
		return EBTNodeResult::Failed;
	}
	
	// Get the current pawn location
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	
	// Find a random point in navigable radius
	FNavLocation NavLocation;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AIController->GetWorld());
	if (!NavSys)
	{
		UE_LOG(LogTemp, Error, TEXT("SimpleExplore: No navigation system!"));
		if (MazeAIController)
		{
			MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
				TEXT("Navigation system not available"));
		}
		return EBTNodeResult::Failed;
	}
	
	// Try to find a random reachable point
	bool bFoundPoint = false;
	
	// First attempt with normal exploration distance
	bFoundPoint = NavSys->GetRandomReachablePointInRadius(CurrentLocation, MaxExplorationDistance, NavLocation);
	
	// If first attempt failed, try with a larger radius as fallback
	if (!bFoundPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("SimpleExplore: Could not find reachable point at distance %.1f, trying larger radius"), 
			MaxExplorationDistance);
		
		// Try with double the exploration distance
		bFoundPoint = NavSys->GetRandomReachablePointInRadius(CurrentLocation, MaxExplorationDistance * 2.0f, NavLocation);
		
		// If still failed, try with a very large radius as last resort
		if (!bFoundPoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("SimpleExplore: Still could not find reachable point, trying maximum radius"));
			bFoundPoint = NavSys->GetRandomReachablePointInRadius(CurrentLocation, 5000.0f, NavLocation);
		}
	}
	
	if (bFoundPoint)
	{
		// Set the exploration target in the blackboard
		BlackboardComp->SetValueAsVector(ExplorationTarget.SelectedKeyName, NavLocation.Location);
		
		// If we're using our custom AI controller, update the current state
		if (MazeAIController)
		{
			MazeAIController->SetCurrentState(EAIState::Exploring);
			
			// If we were in an error state, clear it
			if (MazeAIController->IsInErrorState())
			{
				MazeAIController->TryRecoverFromError();
			}
		}
		
		// Move to the exploration target
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(NavLocation.Location);
		
		// Check if movement started successfully
		if (MoveResult == EPathFollowingRequestResult::Failed)
		{
			UE_LOG(LogTemp, Error, TEXT("SimpleExplore: Failed to start movement!"));
			
			if (MazeAIController)
			{
				MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
					TEXT("Failed to start movement to exploration point"));
			}
			
			return EBTNodeResult::Failed;
		}
		
		// Draw debug sphere to show the target point
		DrawDebugSphere(AIController->GetWorld(), NavLocation.Location, 20.0f, 8, FColor::Blue, false, 3.0f);
		
		return EBTNodeResult::Succeeded;
	}
	
	// If we get here, all attempts to find a navigation point failed
	UE_LOG(LogTemp, Error, TEXT("SimpleExplore: Failed to find ANY reachable point!"));
	
	if (MazeAIController)
	{
		MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
			TEXT("Could not find any reachable navigation points"));
	}
	
	return EBTNodeResult::Failed;
}

FString UBTTask_SimpleExplore::GetStaticDescription() const
{
	return FString::Printf(TEXT("Simple Explore: Max Distance = %.1f"), MaxExplorationDistance);
}