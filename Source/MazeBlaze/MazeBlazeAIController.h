#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MazeBlazeKey.h"
#include "MazeGameDoor.h"
#include "MazeBlazeExit.h"
#include "MazeBlazeAIController.generated.h"

// Enum to define AI states
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Exploring UMETA(DisplayName = "Exploring"),
	SeekingKey UMETA(DisplayName = "Seeking Key"),
	SeekingDoor UMETA(DisplayName = "Seeking Door"),
	GoingToExit UMETA(DisplayName = "Going To Exit")
};

// Enum to define AI error types
UENUM(BlueprintType)
enum class EAIErrorType : uint8
{
	None UMETA(DisplayName = "No Error"),
	AssetMissing UMETA(DisplayName = "Asset Missing"),
	BlackboardInitFailed UMETA(DisplayName = "Blackboard Init Failed"),
	BehaviorTreeStartFailed UMETA(DisplayName = "Behavior Tree Start Failed"),
	NavigationMissing UMETA(DisplayName = "Navigation Missing"),
	PerceptionError UMETA(DisplayName = "Perception Error"),
	TaskExecutionFailed UMETA(DisplayName = "Task Execution Failed")
};

/**
 * AI Controller for MazeBlaze game
 * Handles perception, behavior tree execution, and interaction with maze elements
 * 
 * Includes a comprehensive error handling system that:
 * - Detects common AI errors (navigation, perception, task execution)
 * - Attempts to recover from errors automatically
 * - Provides visual debugging of AI state and errors
 * - Tracks stuck conditions and implements recovery strategies
 */
// Forward declaration
class UBTService_ErrorDetection;

UCLASS()
class MAZEBLAZE_API AMazeBlazeAIController : public AAIController
{
	GENERATED_BODY()

	// Allow BTService_ErrorDetection to access protected members
	friend class UBTService_ErrorDetection;

public:
	// Constructor
	AMazeBlazeAIController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when this controller possesses a pawn
	virtual void OnPossess(APawn* InPawn) override;

	// Called when this controller unpossesses a pawn
	virtual void OnUnPossess() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Behavior tree asset to use
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	// Blackboard asset to use
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BlackboardAsset;

	// Get the current AI state
	UFUNCTION(BlueprintCallable, Category = "AI")
	EAIState GetCurrentState() const;

	// Set the current AI state
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetCurrentState(EAIState NewState);

	// Find the nearest key in the maze
	UFUNCTION(BlueprintCallable, Category = "AI")
	AMazeBlazeKey* FindNearestKey();

	// Find a door that matches the current key
	UFUNCTION(BlueprintCallable, Category = "AI")
	AMazeGameDoor* FindMatchingDoor(AMazeBlazeKey* Key);

	// Find the exit in the maze
	UFUNCTION(BlueprintCallable, Category = "AI")
	AMazeBlazeExit* FindExit();

	// Update perception data in the blackboard
	UFUNCTION(BlueprintCallable, Category = "AI")
	void UpdatePerception();

	// Interact with an object implementing the interactable interface
	UFUNCTION(BlueprintCallable, Category = "AI")
	void InteractWithObject(AActor* InteractableObject);

	// Get the blackboard component
	UBlackboardComponent* GetBlackboardComp() const { return BlackboardComponent; }

	//
	// Error Handling System
	//

	// Current error state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Debug")
	EAIErrorType CurrentErrorState;
	
	// Last error message
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Debug")
	FString LastErrorMessage;
	
	// Report an AI error
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	void ReportAIError(EAIErrorType ErrorType, const FString& ErrorMessage);
	
	// Try to recover from errors
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	bool TryRecoverFromError();
	
	// Check if AI is in error state
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	bool IsInErrorState() const { return CurrentErrorState != EAIErrorType::None; }
	
	// Get debug text for display
	UFUNCTION(BlueprintPure, Category = "AI|Debug")
	FString GetDebugStatusText() const;
	
	// Check if the AI is stuck by comparing current and previous locations
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	bool IsAIStuck();
	
	// Reset the AI state when recovery is needed
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	void ResetAIState();
	
	// Draw debug information to visualize AI state and path
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	void DrawDebugInfo(float Duration = 0.0f);
	
	// Store current location as a valid location for recovery
	UFUNCTION(BlueprintCallable, Category = "AI|Debug")
	void StoreValidLocation();

protected:
	// Behavior tree component
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComponent;

	// Blackboard component
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	// We're using the perception component from the parent class (AAIController)

	// Setup perception system
	void SetupPerceptionSystem();

	// Blackboard key names
	static const FName CurrentTargetKey;
	static const FName CurrentStateKey;
	static const FName VisibleKeysKey;
	static const FName VisibleDoorsKey;
	static const FName CurrentKeyKey;
	static const FName ExitLocationKey;
	static const FName SelfActorKey;
	
private:
	// Time since last recovery attempt
	float TimeSinceLastRecoveryAttempt;
	
	// Maximum time between recovery attempts
	const float RecoveryAttemptInterval = 5.0f;
	
	// Previous location for stuck detection
	FVector PreviousLocation;
	
	// Time at same location for stuck detection
	float StuckTime;
	
	// Threshold for considering AI stuck (in units)
	const float StuckThreshold = 50.0f;
	
	// Maximum time allowed to be stuck before recovery
	const float MaxStuckTime = 3.0f;
	
	// Last valid location for recovery
	FVector LastValidLocation;
	
	// Time in current state
	float TimeInCurrentState;
	
	// Last state for tracking state changes
	EAIState LastState;
	
	// Maximum time allowed in one state
	const float MaxTimeInState = 15.0f;
};