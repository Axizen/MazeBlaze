#include "MazeBlazeKey.h"
#include "MazeBlazeCharacter.h"


AMazeBlazeKey::AMazeBlazeKey()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	RootComponent = InteractionSphere;
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation"));
}

void AMazeBlazeKey::InteractWith_Implementation(AMazeBlazeCharacter* Character)
{
	if (!Character || !IsValid(Character) || !Execute_CanInteractWith(this, Character))
	{
		return;
	}
	Character->PickupKey(this);
}

bool AMazeBlazeKey::CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const
{
	if (!bIsOnGround || !InteractionSphere)
	{
		return false;
	}
	return InteractionSphere->IsOverlappingActor(Character);
}

void AMazeBlazeKey::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
	OutInteractionPoints.Emplace(GetActorLocation());
}

bool AMazeBlazeKey::CanOpen(const AMazeGameDoor* Door) const
{
	if (!Door)
	{
		return false;
	}
	return Door->CanBeOpenedByKey(this);
}

int32 AMazeBlazeKey::GetSignature() const
{
	return Signature;
}

const FName& AMazeBlazeKey::GetKeyName() const
{
	return Name;
}

void AMazeBlazeKey::PickUp_Implementation()
{
	bIsOnGround = false;
	SetActorHiddenInGame(true);
}

void AMazeBlazeKey::DropDownAt_Implementation(const FVector& Location)
{
	SetActorLocation(Location);
	bIsOnGround = true;
	SetActorHiddenInGame(false);
}



