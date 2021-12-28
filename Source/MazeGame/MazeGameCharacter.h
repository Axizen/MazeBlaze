#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MazeGameCharacter.generated.h"

class AMazeGameKey;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupKey, AMazeGameKey*, Key);


UCLASS(config=Game)
class MAZEGAME_API AMazeGameCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMazeGameCharacter();

	UFUNCTION(BlueprintCallable)
	void PickupKey(AMazeGameKey* NewKey);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AMazeGameKey* GetCarriedKey() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCarryingKey() const;

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void InteractWith(AActor* Actor);

	UFUNCTION()
	void MoveForward(const float Value);

	UFUNCTION()
	void MoveRight(const float Value);

	UPROPERTY(BlueprintAssignable, Category = MazeGameCharacter)
	FOnPickupKey OnPickupKey;

protected:

	UPROPERTY()
	AMazeGameKey* CarriedKey = nullptr;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

