#include "BTDecorator_KnowsExit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_KnowsExit::UBTDecorator_KnowsExit()
{
	NodeName = TEXT("Knows Exit");
}

bool UBTDecorator_KnowsExit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Get the blackboard component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	// Get the exit location from the blackboard
	FVector ExitLocation = BlackboardComp->GetValueAsVector(ExitLocationKey.SelectedKeyName);
	
	// Return true if the exit location is valid (not zero)
	return !ExitLocation.IsZero();
}

FString UBTDecorator_KnowsExit::GetStaticDescription() const
{
	return FString::Printf(TEXT("Checks if the AI knows the exit location: %s"), *ExitLocationKey.SelectedKeyName.ToString());
}