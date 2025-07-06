#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MazeBlazeGameMode.generated.h"

/**
 * Game mode for the maze levels that handles player/AI selection
 */
UCLASS()
class MAZEBLAZE_API AMazeBlazeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Constructor
	AMazeBlazeGameMode();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Setup the game mode based on the selected mode (player or AI)
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void SetupGameMode();
	
	// Restart the game with the appropriate character and controller
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void RestartGameWithMode(bool bPlayAsAI);
	
	// Restart all players in the game
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void RestartAllPlayers();

	// The player character class to use when playing as a player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TSubclassOf<class AMazeBlazeCharacter> PlayerCharacterClass;

	// The player controller class to use when playing as a player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TSubclassOf<class AMazeBlazePlayerController> MazePlayerControllerClass;
	
	// The AI character class to use when playing as AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TSubclassOf<class AMazeBlazeCharacter> AICharacterClass;

	// The AI controller class to use when playing as AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Mode")
	TSubclassOf<class AMazeBlazeAIController> AIControllerClass;

};