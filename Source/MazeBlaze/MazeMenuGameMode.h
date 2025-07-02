#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MazeMenuGameMode.generated.h"

// Forward declarations
class UAIExplorationSettingsWidget;

/**
 * Game mode for the main menu that allows selecting between player and AI modes
 */
UCLASS()
class MAZEBLAZE_API AMazeMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Constructor
	AMazeMenuGameMode();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Function to play the maze as a player
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void PlayAsPlayer();

private:
	// Reference to the created menu widget
	UUserWidget* MainMenuWidget;

	// Reference to the AI exploration settings widget
	UAIExplorationSettingsWidget* AIExplorationSettingsWidget;

	// The name of the maze level to load
	FName MazeLevelName;
};