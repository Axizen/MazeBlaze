#include "BTTask_PickupKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_PickupKey::UBTTask_PickupKey()
{
	NodeName = TEXT("Pickup Key");
}

EBTNodeResult::Type UBTTask_PickupKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	// Get the key to pick up from the blackboard
	AMazeBlazeKey* Key = Cast<AMazeBlazeKey>(BlackboardComp->GetValueAsObject(KeyToPickup.SelectedKeyName));
	if (!Key)
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
	
	// Check if we can interact with the key
	if (!Key->CanInteractWith_Implementation(Character))
	{
		return EBTNodeResult::Failed;
	}
	
	// Interact with the key
	Key->InteractWith_Implementation(Character);
	
	// If we're using our custom AI controller, update perception
	AMazeBlazeAIController* MazeAIController = Cast<AMazeBlazeAIController>(AIController);
	if (MazeAIController)
	{
		MazeAIController->UpdatePerception();
	}
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_PickupKey::GetStaticDescription() const
{
	return FString::Printf(TEXT("Pickup Key: %s"), *KeyToPickup.SelectedKeyName.ToString());
}