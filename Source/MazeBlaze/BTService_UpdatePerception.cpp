#include "BTService_UpdatePerception.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_UpdatePerception::UBTService_UpdatePerception()
{
	NodeName = TEXT("Update Perception");
	
	// Set the service to tick every 0.5 seconds
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_UpdatePerception::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}
	
	// If we're using our custom AI controller, update perception
	AMazeBlazeAIController* MazeAIController = Cast<AMazeBlazeAIController>(AIController);
	if (MazeAIController)
	{
		MazeAIController->UpdatePerception();
	}
}

FString UBTService_UpdatePerception::GetStaticDescription() const
{
	return FString::Printf(TEXT("Updates the AI's perception of keys, doors, and exits every %.1f seconds"), Interval);
}