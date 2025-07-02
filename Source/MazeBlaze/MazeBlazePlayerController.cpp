#include "MazeBlazePlayerController.h"

void AMazeBlazePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveForward", this, &AMazeBlazePlayerController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMazeBlazePlayerController::MoveRight);

		InputComponent->BindAxis("Turn", this, &AMazeBlazePlayerController::Turn);
		InputComponent->BindAxis("TurnRate", this, &AMazeBlazePlayerController::TurnAtRate);
		InputComponent->BindAxis("LookUp", this, &AMazeBlazePlayerController::LookUp);
		InputComponent->BindAxis("LookUpRate", this, &AMazeBlazePlayerController::LookUpAtRate);

		InputComponent->BindAction("Interact", IE_Pressed, this, &AMazeBlazePlayerController::Interact);
	}
}

void AMazeBlazePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CharacterCache = Cast<AMazeBlazeCharacter>(InPawn);
}

void AMazeBlazePlayerController::OnUnPossess()
{
	CharacterCache = nullptr;
	Super::OnUnPossess();
}

void AMazeBlazePlayerController::MoveForward(const float Value)
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->MoveForward(Value);
	}
}

void AMazeBlazePlayerController::MoveRight(const float Value)
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->MoveRight(Value);
	}
}

void AMazeBlazePlayerController::Turn(const float Rate)
{
	AddYawInput(Rate);
}

void AMazeBlazePlayerController::TurnAtRate(const float Rate)
{
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMazeBlazePlayerController::LookUp(const float Rate)
{
	AddPitchInput(Rate);
}

void AMazeBlazePlayerController::LookUpAtRate(const float Rate)
{
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMazeBlazePlayerController::Interact()
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->Interact();
	}
}
