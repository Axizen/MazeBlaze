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
	
	// Create the main menu widget
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			
			// Set input mode to UI only
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				PC->SetInputMode(FInputModeUIOnly());
				PC->bShowMouseCursor = true;
			}
		}
	}
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

void AMazeMenuGameMode::PlayAsAI()
{
	// Get the game instance
	UMazeBlazeGameInstance* GameInstance = Cast<UMazeBlazeGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// Set play as AI
		GameInstance->SetPlayAsAI(true);
	}
	
	// Open the maze level
	UGameplayStatics::OpenLevel(this, MazeLevelName);
}