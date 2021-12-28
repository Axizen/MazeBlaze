#pragma once
#include "MazeGameCharacter.h"
#include "MazeGameInteractable.h"
#include "Components/BoxComponent.h"
#include "MazeGameDoor.generated.h"

class AMazeGameKey;
class AMazeGameBot;

UCLASS(BlueprintType)
class MAZEGAME_API AMazeGameDoor : public AActor, public IMazeGameInteractableInterface
{
	GENERATED_BODY()

public:

	AMazeGameDoor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameDoor)
	bool IsOpen() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameDoor)
	bool CanBeOpenedByKey(const AMazeGameKey* Key) const;

	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const override;
	virtual bool CanInteractWith_Implementation(const AMazeGameCharacter* Character) const override;
	virtual void InteractWith_Implementation(AMazeGameCharacter* Character) override;

	USceneComponent* GetInteractionPointA() { return InteractionPointA; }

	USceneComponent* GetInteractionPointB() { return InteractionPointB; }

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameDoor)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameDoor)
	UBoxComponent* InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameDoor)
	USceneComponent* InteractionPointA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameDoor)
	USceneComponent* InteractionPointB;

	UPROPERTY()
	bool bIsOpen = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGameDoor)
	int32 Mask = 0;
};
