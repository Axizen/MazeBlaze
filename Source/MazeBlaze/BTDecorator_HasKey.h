#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MazeBlazeCharacter.h"
#include "BTDecorator_HasKey.generated.h"

/**
 * Behavior Tree Decorator that checks if the AI is carrying a key
 */
UCLASS()
class MAZEBLAZE_API UBTDecorator_HasKey : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HasKey();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};