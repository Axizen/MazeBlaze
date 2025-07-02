#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MazeBlazeGameInstance.generated.h"

/**
 * Enum defining different AI exploration systems
 */
UENUM(BlueprintType)
enum class EAIExplorationSystem : uint8
{
	Frontier UMETA(DisplayName = "Frontier-Based Exploration"),
	GraphBased UMETA(DisplayName = "Graph-Based Navigation"),
	HierarchicalAStar UMETA(DisplayName = "Hierarchical A* with Memory"),
	PotentialField UMETA(DisplayName = "Potential Field Navigation"),
	WallFollowing UMETA(DisplayName = "Memory-Augmented Wall Following"),
	VisibilityGraph UMETA(DisplayName = "Visibility Graph Navigation")
};

/**
 * Game instance class that stores game-wide variables and state
 */
UCLASS()
class MAZEBLAZE_API UMazeBlazeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Constructor
	UMazeBlazeGameInstance();

	// Whether to play as AI or player
	UPROPERTY(BlueprintReadWrite, Category = "Game Settings")
	bool bPlayAsAI;

	// The AI exploration system to use
	UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
	EAIExplorationSystem AIExplorationSystem;

	// Set whether to play as AI
	UFUNCTION(BlueprintCallable, Category = "Game Settings")
	void SetPlayAsAI(bool bNewValue);

	// Get whether to play as AI
	UFUNCTION(BlueprintPure, Category = "Game Settings")
	bool GetPlayAsAI() const;
	
	// Set the AI exploration system
	UFUNCTION(BlueprintCallable, Category = "AI Settings")
	void SetAIExplorationSystem(EAIExplorationSystem NewSystem);

	// Get the current AI exploration system
	UFUNCTION(BlueprintPure, Category = "AI Settings")
	EAIExplorationSystem GetAIExplorationSystem() const;
	
	// Get description of the current AI exploration system
	UFUNCTION(BlueprintPure, Category = "AI Settings")
	FText GetExplorationSystemDescription() const;
	
	// Get pros of the current AI exploration system
	UFUNCTION(BlueprintPure, Category = "AI Settings")
	FText GetExplorationSystemPros() const;
	
	// Get cons of the current AI exploration system
	UFUNCTION(BlueprintPure, Category = "AI Settings")
	FText GetExplorationSystemCons() const;
};