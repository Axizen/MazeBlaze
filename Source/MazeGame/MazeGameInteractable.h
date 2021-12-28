#pragma once
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "MazeGameInteractable.generated.h"

class AMazeGameCharacter;

UINTERFACE(BlueprintType)
class MAZEGAME_API UMazeGameInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class MAZEGAME_API IMazeGameInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
	bool CanInteractWith(const AMazeGameCharacter* Character) const;
	virtual bool CanInteractWith_Implementation(const AMazeGameCharacter* Character) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
    void InteractWith(AMazeGameCharacter* Character);
	virtual void InteractWith_Implementation(AMazeGameCharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = MazeGameInteractable)
    void GetInteractionPoints(TArray<FVector>& OutInteractionPoints) const;
	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const;

};
