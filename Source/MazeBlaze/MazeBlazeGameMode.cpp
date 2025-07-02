#include "MazeBlazeGameMode.h"
#include "MazeBlazeGameInstance.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazePlayerController.h"
#include "Kismet/GameplayStatics.h"

AMazeBlazeGameMode::AMazeBlazeGameMode()
{
	// Set default values
	PlayerCharacterClass = nullptr;
	MazePlayerControllerClass = nullptr;
}

void AMazeBlazeGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup the game mode based on the selected mode
	SetupGameMode();
}

void AMazeBlazeGameMode::SetupGameMode()
{
	// Get the game instance
	UMazeBlazeGameInstance* GameInstance = Cast<UMazeBlazeGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		// If we can't get the game instance, default to player mode
		UE_LOG(LogTemp, Warning, TEXT("MazeBlazeGameMode: Failed to get game instance, defaulting to player mode"));
		if (PlayerCharacterClass)
		{
			DefaultPawnClass = PlayerCharacterClass;
		}
		if (MazePlayerControllerClass)
		{
			PlayerControllerClass = MazePlayerControllerClass;
		}
		return;
	}
	
	// Check if we should play as AI
	if (GameInstance->GetPlayAsAI())
	{
		// Set AI character and controller classes
		UE_LOG(LogTemp, Display, TEXT("MazeBlazeGameMode: Setting up AI mode"));

	}
	else
	{
		// Set player character and controller classes
		UE_LOG(LogTemp, Display, TEXT("MazeBlazeGameMode: Setting up player mode"));
		if (PlayerCharacterClass)
		{
			DefaultPawnClass = PlayerCharacterClass;
		}
		if (MazePlayerControllerClass)
		{
			PlayerControllerClass = MazePlayerControllerClass;
		}
	}
	
	// If we already have a player controller, restart the player
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		RestartPlayer(PC);
	}
}