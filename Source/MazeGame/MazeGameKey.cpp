#include "MazeGameKey.h"
#include "MazeGameCharacter.h"
#include "MazeGameBot.h"



AMazeGameKey::AMazeGameKey()
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

void AMazeGameKey::InteractWith_Implementation(AMazeGameCharacter* Character)
{
	if (!Character || !IsValid(Character) || !Execute_CanInteractWith(this, Character))
	{
		return;
	}
	Character->PickupKey(this);
}

bool AMazeGameKey::CanInteractWith_Implementation(const AMazeGameCharacter* Character) const
{
	if (!bIsOnGround || !InteractionSphere)
	{
		return false;
	}
	return InteractionSphere->IsOverlappingActor(Character);
}

void AMazeGameKey::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
	OutInteractionPoints.Emplace(GetActorLocation());
}

bool AMazeGameKey::CanOpen(const AMazeGameDoor* Door) const
{
	if (!Door)
	{
		return false;
	}
	return Door->CanBeOpenedByKey(this);
}

int32 AMazeGameKey::GetSignature() const
{
	return Signature;
}

const FName& AMazeGameKey::GetKeyName() const
{
	return Name;
}

void AMazeGameKey::PickUp_Implementation()
{
	bIsOnGround = false;
	SetActorHiddenInGame(true);
}

void AMazeGameKey::DropDownAt_Implementation(const FVector& Location)
{
	SetActorLocation(Location);
	bIsOnGround = true;
	SetActorHiddenInGame(false);
}

