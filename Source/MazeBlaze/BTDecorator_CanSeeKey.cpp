#include "BTDecorator_CanSeeKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_CanSeeKey::UBTDecorator_CanSeeKey()
{
	NodeName = TEXT("Can See Key");
}

bool UBTDecorator_CanSeeKey::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Get the blackboard component
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	// Get the visible key from the blackboard
	AMazeBlazeKey* VisibleKey = Cast<AMazeBlazeKey>(BlackboardComp->GetValueAsObject(VisibleKeyKey.SelectedKeyName));
	
	// Return true if we can see a key
	return VisibleKey != nullptr;
}

FString UBTDecorator_CanSeeKey::GetStaticDescription() const
{
	return FString::Printf(TEXT("Checks if the AI can see a key: %s"), *VisibleKeyKey.SelectedKeyName.ToString());
}