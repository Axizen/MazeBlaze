#pragma once
#include "MazeBlazeInteractableInterface.h"
#include "Components/BoxComponent.h"
#include "MazeBlazeExit.generated.h"

UCLASS()
class MAZEBLAZE_API AMazeBlazeExit : public AActor, public IMazeBlazeInteractableInterface
{
	GENERATED_BODY()
	
public:

	AMazeBlazeExit();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = MazeGameExit)
	void OnFinalLevelExit();
	virtual void OnFinalLevelExit_Implementation();

	virtual bool CanInteractWith_Implementation(const AMazeBlazeCharacter* Character) const override;
	virtual void InteractWith_Implementation(AMazeBlazeCharacter* Character) override;
	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> NextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFinalLevel = true;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameExit)
	UBoxComponent* Box;

};
