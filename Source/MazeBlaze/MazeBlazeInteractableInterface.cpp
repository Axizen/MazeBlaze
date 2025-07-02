#include "MazeBlazeInteractableInterface.h"

bool IMazeBlazeInteractableInterface::CanInteractWith_Implementation(const AMazeBlazeCharacter*) const
{
	return false;
}

void IMazeBlazeInteractableInterface::InteractWith_Implementation(AMazeBlazeCharacter* Character)
{
}

void IMazeBlazeInteractableInterface::GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const
{
	OutInteractionPoints.Empty();
}
