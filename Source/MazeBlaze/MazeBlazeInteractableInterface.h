#pragma once
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "MazeBlazeInteractableInterface.generated.h"

class AMazeBlazeCharacter;

UINTERFACE(BlueprintType)
class UMazeBlazeInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class MAZEBLAZE_API IMazeBlazeInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
	bool CanInteractWith(const AMazeBlazeCharacter* Character) const;
	virtual bool CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
	void InteractWith(AMazeBlazeCharacter* Character);
	virtual void InteractWith_Implementation(AMazeBlazeCharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
	void GetInteractionPoints(TArray<FVector>& OutInteractionPoints) const;
	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const;
};
