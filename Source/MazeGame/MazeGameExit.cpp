#include "MazeGameExit.h"
#include "MazeGameCharacter.h"
#include "MazeGameBot.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMazeGameExit::AMazeGameExit()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = Box;

	ExitReference = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitReference"));
	ExitReference->SetupAttachment(RootComponent);
	ExitReference->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ExitReference->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExitReference->SetVisibility(false);

}

void AMazeGameExit::OnFinalLevelExit_Implementation()
{
	
}

bool AMazeGameExit::CanInteractWith_Implementation(const AMazeGameCharacter* Character) const
{
	if (!Box || !Character)
	{
		return false;
	}
	return Box->IsOverlappingActor(Character);
}

void AMazeGameExit::InteractWith_Implementation(AMazeGameCharacter* Character)
{
	if (!Execute_CanInteractWith(this, Character))
	{
		return;
	}
	if (bIsFinalLevel)
	{
		OnFinalLevelExit();
	} else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), NextLevel, false);	
	}
}

void AMazeGameExit::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
	OutInteractionPoints.Emplace(GetActorLocation());
}