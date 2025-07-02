// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MazeBlazeCharacter.generated.h"

class AMazeBlazeKey;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickupKey, AMazeBlazeKey*, Key);


UCLASS(config = Game)
class MAZEBLAZE_API AMazeBlazeCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AMazeBlazeCharacter();

	UFUNCTION(BlueprintCallable)
	void PickupKey(AMazeBlazeKey* NewKey);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AMazeBlazeKey* GetCarriedKey() const;

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
	AMazeBlazeKey* CarriedKey = nullptr;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
