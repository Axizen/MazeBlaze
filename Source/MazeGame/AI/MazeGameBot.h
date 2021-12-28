// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeGameCharacter.h"
#include "MazeGameBot.generated.h"


UCLASS()
class MAZEGAME_API AMazeGameBot : public AMazeGameCharacter
{
	GENERATED_BODY()
	
public:
	AMazeGameBot();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MazeGame")
	bool bSearchForDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MazeGame")
	bool bSearchForExit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MazeGame")
	bool bSearchForAnotherKey;

	//Holds all the keys the Bot picks up 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MazeGame")
	TArray<AMazeGameKey*> MazeKeyCollection;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "MazeGame")
	void WalkThroughDoor(class AMazeGameDoor* OpenedDoor, class ABotController* BotCon);

	UFUNCTION(BlueprintCallable)
	void BotPickupKey(AMazeGameKey* NewKey);

	UFUNCTION(BlueprintCallable)
	void BotInteract();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MazeGame")
	FVector LastKeyLocation;
};
