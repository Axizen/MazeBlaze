#include "MazeMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/GameInstance.h"
#include "MazeBlazeGameInstance.h"

AMazeMenuGameMode::AMazeMenuGameMode()
{
	// Set default values
	MainMenuWidget = nullptr;

	
	// Set the default maze level name
	MazeLevelName = TEXT("Maze_0_Intro");
}

void AMazeMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMazeMenuGameMode::PlayAsPlayer()
{
	// Get the game instance
	UMazeBlazeGameInstance* GameInstance = Cast<UMazeBlazeGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// Set play as player (not AI)
		GameInstance->SetPlayAsAI(false);
	}
	
	// Open the maze level
	UGameplayStatics::OpenLevel(this, MazeLevelName);
}

