#include "BTTask_OpenDoor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_OpenDoor::UBTTask_OpenDoor()
{
	NodeName = TEXT("Open Door");
}

EBTNodeResult::Type UBTTask_OpenDoor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	// Get the door to open from the blackboard
	AMazeGameDoor* Door = Cast<AMazeGameDoor>(BlackboardComp->GetValueAsObject(DoorToOpen.SelectedKeyName));
	if (!Door)
	{
		return EBTNodeResult::Failed;
	}
	
	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	// Cast to MazeBlazeCharacter
	AMazeBlazeCharacter* Character = Cast<AMazeBlazeCharacter>(ControlledPawn);
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}
	
	// Check if we can interact with the door
	if (!Door->CanInteractWith_Implementation(Character))
	{
		return EBTNodeResult::Failed;
	}
	
	// Interact with the door
	Door->InteractWith_Implementation(Character);
	
	// If we're using our custom AI controller, update perception
	AMazeBlazeAIController* MazeAIController = Cast<AMazeBlazeAIController>(AIController);
	if (MazeAIController)
	{
		MazeAIController->UpdatePerception();
	}
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_OpenDoor::GetStaticDescription() const
{
	return FString::Printf(TEXT("Open Door: %s"), *DoorToOpen.SelectedKeyName.ToString());
}