#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MazeBlazeAIController.h"
#include "BTService_UpdatePerception.generated.h"

/**
 * Behavior Tree Service that continuously updates the AI's perception
 */
UCLASS()
class MAZEBLAZE_API UBTService_UpdatePerception : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdatePerception();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;
};