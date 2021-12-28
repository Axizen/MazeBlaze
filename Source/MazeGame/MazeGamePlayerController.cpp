#include "MazeGamePlayerController.h"


void AMazeGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveForward", this, &AMazeGamePlayerController::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMazeGamePlayerController::MoveRight);
	
		InputComponent->BindAxis("Turn", this, &AMazeGamePlayerController::Turn);
		InputComponent->BindAxis("TurnRate", this, &AMazeGamePlayerController::TurnAtRate);
		InputComponent->BindAxis("LookUp", this, &AMazeGamePlayerController::LookUp);
		InputComponent->BindAxis("LookUpRate", this, &AMazeGamePlayerController::LookUpAtRate);

		InputComponent->BindAction("Interact", IE_Pressed, this, &AMazeGamePlayerController::Interact);		
	}
}

void AMazeGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CharacterCache = Cast<AMazeGameCharacter>(InPawn);
}

void AMazeGamePlayerController::OnUnPossess()
{
	CharacterCache = nullptr;
	Super::OnUnPossess();
}

void AMazeGamePlayerController::MoveForward(const float Value)
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->MoveForward(Value);	
	}
}

void AMazeGamePlayerController::MoveRight(const float Value)
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->MoveRight(Value);
	}
}

void AMazeGamePlayerController::Turn(const float Rate)
{
	AddYawInput(Rate);
}

void AMazeGamePlayerController::TurnAtRate(const float Rate)
{
	AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMazeGamePlayerController::LookUp(const float Rate)
{
	AddPitchInput(Rate);
}

void AMazeGamePlayerController::LookUpAtRate(const float Rate)
{
	AddPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMazeGamePlayerController::Interact()
{
	if (CharacterCache && IsValid(CharacterCache))
	{
		CharacterCache->Interact();		
	}
}
