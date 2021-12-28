#pragma once
#include "MazeGameCharacter.h"
#include "MazeGameDoor.h"
#include "MazeGameInteractable.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "MazeGameKey.generated.h"

UCLASS(BlueprintType)
class MAZEGAME_API AMazeGameKey : public AActor, public IMazeGameInteractableInterface
{
	GENERATED_BODY()

public:

	AMazeGameKey();

	virtual void InteractWith_Implementation(AMazeGameCharacter* Character) override;
	virtual bool CanInteractWith_Implementation(const AMazeGameCharacter* Character) const override;
	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameKey)
    bool CanOpen(const AMazeGameDoor* Door) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameKey)
	int32 GetSignature() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MazeGameKey)
	const FName& GetKeyName() const; 

	UFUNCTION(BlueprintNativeEvent, Category = MazeGameKey)
	void PickUp();
	virtual void PickUp_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = MazeGameKey)
	void DropDownAt(const FVector& Location);
	virtual void DropDownAt_Implementation(const FVector& Location);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameKey)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameKey)
	USphereComponent* InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameKey)
	URotatingMovementComponent* RotatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGameKey)
	int32 Signature = 0;
	
	UPROPERTY()
	bool bIsOnGround = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGameKey)
	FName Name;

};
