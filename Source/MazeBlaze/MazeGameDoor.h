#pragma once
#include "MazeBlazeCharacter.h"
#include "MazeBlazeInteractableInterface.h"
#include "Components/BoxComponent.h"
#include "MazeGameDoor.generated.h"

UCLASS()
class MAZEBLAZE_API AMazeGameDoor : public AActor, public IMazeBlazeInteractableInterface
{
	GENERATED_BODY()
	
public:

	AMazeGameDoor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameDoor)
	bool IsOpen() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameDoor)
	bool CanBeOpenedByKey(const AMazeBlazeKey* Key) const;

	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const override;
	virtual bool CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const override;
	virtual void InteractWith_Implementation(AMazeBlazeCharacter* Character) override;

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
