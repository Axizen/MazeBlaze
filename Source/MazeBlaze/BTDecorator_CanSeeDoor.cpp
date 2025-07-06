#include "BTDecorator_CanSeeDoor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_CanSeeDoor::UBTDecorator_CanSeeDoor()
{
	NodeName = TEXT("Can See Door");
}

bool UBTDecorator_CanSeeDoor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Get the blackboard component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	// Get the visible door from the blackboard
	AMazeGameDoor* VisibleDoor = Cast<AMazeGameDoor>(BlackboardComp->GetValueAsObject(VisibleDoorKey.SelectedKeyName));
	
	// Return true if we can see a door
	return VisibleDoor != nullptr;
}

FString UBTDecorator_CanSeeDoor::GetStaticDescription() const
{
	return FString::Printf(TEXT("Checks if the AI can see a door: %s"), *VisibleDoorKey.SelectedKeyName.ToString());
}