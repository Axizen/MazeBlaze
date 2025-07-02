#include "MazeBlazeCharacter.h"
#include "MazeBlazeKey.h"
#include "MazeBlazeInteractableInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AMazeBlazeCharacter::AMazeBlazeCharacter()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->InitCapsuleSize(42.f, 96.0f);
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
		Movement->JumpZVelocity = 600.f;
		Movement->AirControl = 0.2f;
	}

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

void AMazeBlazeCharacter::PickupKey(AMazeBlazeKey* NewKey)
{
	if (!NewKey || !IsValid(NewKey) || !NewKey->Execute_CanInteractWith(NewKey, this))
	{
		return;
	}
	if (CarriedKey && IsValid(CarriedKey))
	{
		CarriedKey->DropDownAt(NewKey->GetActorLocation());
	}
	NewKey->PickUp();
	CarriedKey = NewKey;
	OnPickupKey.Broadcast(CarriedKey);
}

AMazeBlazeKey* AMazeBlazeCharacter::GetCarriedKey() const
{
	return CarriedKey;
}

void AMazeBlazeCharacter::Interact()
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		if (!Actor || !IsValid(Actor))
		{
			continue;
		}

		IMazeBlazeInteractableInterface* Interactable = Cast<IMazeBlazeInteractableInterface>(Actor);
		if (!Interactable || !Interactable->Execute_CanInteractWith(Actor, this))
		{
			continue;
		}

		Interactable->Execute_InteractWith(Actor, this);
		break;
	}
}

void AMazeBlazeCharacter::InteractWith(AActor* Actor)
{
	if (!Actor || !IsValid(Actor))
	{
		return;
	}

	IMazeBlazeInteractableInterface* Interactable = Cast<IMazeBlazeInteractableInterface>(Actor);
	if (!Interactable || !Interactable->Execute_CanInteractWith(Actor, this))
	{
		return;
	}

	Interactable->Execute_InteractWith(Actor, this);
}

bool AMazeBlazeCharacter::IsCarryingKey() const
{
	return CarriedKey && IsValid(CarriedKey);
}

void AMazeBlazeCharacter::MoveForward(const float Value)
{
	if (Controller && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMazeBlazeCharacter::MoveRight(const float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}
