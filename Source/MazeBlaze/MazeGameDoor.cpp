#include "MazeGameDoor.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazeKey.h"

AMazeGameDoor::AMazeGameDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = InteractionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetVisibility(true);
	Mesh->SetMobility(EComponentMobility::Movable);

	InteractionPointA = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPointA"));
	InteractionPointA->SetupAttachment(RootComponent);

	InteractionPointB = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPointB"));
	InteractionPointB->SetupAttachment(RootComponent);
}

bool AMazeGameDoor::IsOpen() const
{
	return bIsOpen;
}

bool AMazeGameDoor::CanBeOpenedByKey(const AMazeBlazeKey* Key) const
{
	if (!Key)
	{
		return false;
	}
	const int32 Signature = Key->GetSignature();
	return Signature & Mask;
}

void AMazeGameDoor::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
	if (InteractionPointA)
	{
		OutInteractionPoints.Emplace(InteractionPointA->GetComponentLocation());
	}
	if (InteractionPointB)
	{
		OutInteractionPoints.Emplace(InteractionPointB->GetComponentLocation());
	}
}

bool AMazeGameDoor::CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const
{
	if (!InteractionBox)
	{
		return false;
	}
	return InteractionBox->IsOverlappingActor(Character) && !bIsOpen;
}

void AMazeGameDoor::InteractWith_Implementation(AMazeBlazeCharacter* Character)
{
	if (!Character || !Execute_CanInteractWith(this, Character))
	{
		return;
	}

	if (!CanBeOpenedByKey(Character->GetCarriedKey()))
	{
		return;
	}

	bIsOpen = true;
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetVisibility(false);
	}
}

