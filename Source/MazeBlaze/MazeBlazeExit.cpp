#include "MazeBlazeExit.h"
#include "MazeBlazeCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMazeBlazeExit::AMazeBlazeExit()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = Box;
}

void AMazeBlazeExit::OnFinalLevelExit_Implementation()
{

}

bool AMazeBlazeExit::CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const
{
	if (!Box || !Character)
	{
		return false;
	}
	return Box->IsOverlappingActor(Character);
}

void AMazeBlazeExit::InteractWith_Implementation(AMazeBlazeCharacter* Character)
{
	if (!Execute_CanInteractWith(this, Character))
	{
		return;
	}
	if (bIsFinalLevel)
	{
		OnFinalLevelExit();
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel, false);
	}
}

void AMazeBlazeExit::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
	OutInteractionPoints.Emplace(GetActorLocation());
}

