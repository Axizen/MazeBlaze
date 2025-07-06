#include "BTDecorator_HasKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_HasKey::UBTDecorator_HasKey()
{
	NodeName = TEXT("Has Key");
}

bool UBTDecorator_HasKey::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}
	
	// Get the controlled pawn
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}
	
	// Cast to MazeBlazeCharacter
	AMazeBlazeCharacter* Character = Cast<AMazeBlazeCharacter>(ControlledPawn);
	if (!Character)
	{
		return false;
	}
	
	// Check if the character is carrying a key
	return Character->IsCarryingKey();
}

FString UBTDecorator_HasKey::GetStaticDescription() const
{
	return TEXT("Checks if the AI is carrying a key");
}