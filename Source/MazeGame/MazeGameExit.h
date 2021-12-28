#pragma once
#include "MazeGameInteractable.h"
#include "Components/BoxComponent.h"
#include "MazeGameExit.generated.h"

UCLASS(BlueprintType)
class MAZEGAME_API AMazeGameExit : public AActor, public IMazeGameInteractableInterface
{
	GENERATED_BODY()

public:

	AMazeGameExit();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = MazeGameExit)
	void OnFinalLevelExit();
	virtual void OnFinalLevelExit_Implementation();
	
    virtual bool CanInteractWith_Implementation(const AMazeGameCharacter* Character) const override;
    virtual void InteractWith_Implementation(AMazeGameCharacter* Character) override;
	virtual void GetInteractionPoints_Implementation(TArray<FVector>& OutInteractionPoints) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> NextLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFinalLevel = true;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameExit)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MazeGameDoor)
	UBoxComponent* ExitReference;

};
