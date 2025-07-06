#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MazeBlazeKey.h"
#include "BTDecorator_CanSeeKey.generated.h"

/**
 * Behavior Tree Decorator that checks if the AI can see a key
 */
UCLASS()
class MAZEBLAZE_API UBTDecorator_CanSeeKey : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanSeeKey();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

	// The blackboard key that holds the visible key
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector VisibleKeyKey;
};