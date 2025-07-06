#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
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

	// Function to watch the AI solve the maze
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void PlayAsAI();
	
	// The widget class to use for the main menu
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
	// Reference to the created menu widget
	UUserWidget* MainMenuWidget;

	// Reference to the AI exploration settings widget
	UAIExplorationSettingsWidget* AIExplorationSettingsWidget;

	// The name of the maze level to load
	FName MazeLevelName;
};