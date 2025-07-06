// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MazeBlazeCharacter.h"
#include "MazeBlazeAICharacter.generated.h"

class AMazeBlazeKey;
class AMazeGameDoor;
class AMazeBlazeExit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorOpened, AMazeGameDoor*, Door);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitReached, AMazeBlazeExit*, Exit);

/**
 * AI-controlled character for the MazeBlaze game
 */
UCLASS()
class MAZEBLAZE_API AMazeBlazeAICharacter : public AMazeBlazeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMazeBlazeAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initialize the AI character with the appropriate controller
	UFUNCTION(BlueprintCallable, Category = "AI")
	void InitializeAICharacter();

	// Event called when a key is collected
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnKeyCollected(AMazeBlazeKey* Key);

	// Event called when a door is opened
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnDoorOpened(AMazeGameDoor* Door);

	// Event called when the exit is reached
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnReachedExit(AMazeBlazeExit* Exit);

	// Get performance metrics as a formatted string
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	FString GetPerformanceMetrics() const;

	// Update the visualization of the current path (for debugging)
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdatePathVisualization(const TArray<FVector>& Path);

	// Detect if the AI is backtracking (going back to previously visited locations)
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool DetectBacktracking(const FVector& NewLocation);

	// Set the current path the AI is following
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetCurrentPath(const TArray<FVector>& NewPath);

	// Delegate for key pickup events
	UPROPERTY(BlueprintAssignable, Category = "AI Events")
	FOnPickupKey OnKeyPickedUp;

	// Delegate for door opened events
	UPROPERTY(BlueprintAssignable, Category = "AI Events")
	FOnDoorOpened OnDoorOpenedEvent;

	// Delegate for exit reached events
	UPROPERTY(BlueprintAssignable, Category = "AI Events")
	FOnExitReached OnExitReached;

protected:
	// Number of keys collected during the run
	UPROPERTY(BlueprintReadOnly, Category = "AI Metrics")
	int32 KeysCollected;

	// Number of doors opened during the run
	UPROPERTY(BlueprintReadOnly, Category = "AI Metrics")
	int32 DoorsOpened;

	// Current path the AI is following
	UPROPERTY(BlueprintReadOnly, Category = "AI Navigation")
	TArray<FVector> CurrentPath;

	// Instances of backtracking detected
	UPROPERTY(BlueprintReadOnly, Category = "AI Metrics")
	int32 BacktrackingInstances;

	// Whether this character is controlled by AI
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsControlledByAI;

	// Time taken since the level started
	UPROPERTY(BlueprintReadOnly, Category = "AI Metrics")
	float TimeTaken;

	// Optional mesh component for visualizing AI path
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Visualization")
	UStaticMeshComponent* AIVisualMesh;

	// Whether the AI has reached the exit
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bReachedExit;
};