#include "MazeGameInteractable.h"

bool IMazeGameInteractableInterface::CanInteractWith_Implementation(const AMazeGameCharacter*) const
{
	return false;
}

void IMazeGameInteractableInterface::InteractWith_Implementation(AMazeGameCharacter* Character)
{
}

void IMazeGameInteractableInterface::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
}
