#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_KnowsExit.generated.h"

/**
 * Behavior Tree Decorator that checks if the AI knows the exit location
 */
UCLASS()
class MAZEBLAZE_API UBTDecorator_KnowsExit : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_KnowsExit();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the exit location
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ExitLocationKey;
};