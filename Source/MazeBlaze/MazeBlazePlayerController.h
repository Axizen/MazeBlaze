#pragma once

#include "MazeBlazeCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MazeBlazePlayerController.generated.h"


UCLASS(BlueprintType)
class MAZEBLAZE_API AMazeBlazePlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

protected:

	virtual void SetupInputComponent() override;

	void MoveForward(const float Value);
	void MoveRight(const float Value);

	void Turn(const float Rate);
	void TurnAtRate(const float Rate);
	void LookUp(const float Rate);
	void LookUpAtRate(const float Rate);

	void Interact();
private:

	UPROPERTY(Transient)
	AMazeBlazeCharacter* CharacterCache;
};
