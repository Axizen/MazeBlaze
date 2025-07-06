#include "MazeBlazeGameMode.h"
#include "MazeBlazeGameInstance.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazePlayerController.h"
#include "MazeBlazeAIController.h"
#include "Kismet/GameplayStatics.h"

AMazeBlazeGameMode::AMazeBlazeGameMode()
{
	// Set default values
	PlayerCharacterClass = nullptr;
	MazePlayerControllerClass = nullptr;
	AICharacterClass = nullptr;
	AIControllerClass = nullptr;
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
		
		if (AICharacterClass)
		{
			DefaultPawnClass = AICharacterClass;
		}
		else if (PlayerCharacterClass)
		{
			// Fall back to player character class if AI character class is not set
			DefaultPawnClass = PlayerCharacterClass;
		}
		
		if (AIControllerClass)
		{
			PlayerControllerClass = AIControllerClass;
		}
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
	
	// Restart all players to apply the changes
	RestartAllPlayers();
}

void AMazeBlazeGameMode::RestartGameWithMode(bool bPlayAsAI)
{
	// Get the game instance
	UMazeBlazeGameInstance* GameInstance = Cast<UMazeBlazeGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("MazeBlazeGameMode: Failed to get game instance, cannot restart game"));
		return;
	}
	
	// Set the play mode in the game instance
	GameInstance->SetPlayAsAI(bPlayAsAI);
	
	// Setup the game mode with the new settings
	SetupGameMode();
}

void AMazeBlazeGameMode::RestartAllPlayers()
{
	// Get all player controllers
	TArray<APlayerController*> PlayerControllers;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			// Destroy existing pawns
			if (PC->GetPawn())
			{
				PC->GetPawn()->Destroy();
			}
			
			// Restart the player
			RestartPlayer(PC);
		}
	}
}