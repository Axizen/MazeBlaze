#include "MazeBlazeAIController.h"
#include "MazeBlazeCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "MazeBlazeGameInstance.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

// Define blackboard key names
const FName AMazeBlazeAIController::CurrentTargetKey = TEXT("CurrentTarget");
const FName AMazeBlazeAIController::CurrentStateKey = TEXT("CurrentState");
const FName AMazeBlazeAIController::VisibleKeysKey = TEXT("VisibleKeys");
const FName AMazeBlazeAIController::VisibleDoorsKey = TEXT("VisibleDoors");
const FName AMazeBlazeAIController::CurrentKeyKey = TEXT("CurrentKey");
const FName AMazeBlazeAIController::ExitLocationKey = TEXT("ExitLocation");
const FName AMazeBlazeAIController::SelfActorKey = TEXT("SelfActor");

AMazeBlazeAIController::AMazeBlazeAIController()
{
	// Create behavior tree component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	
	// Create blackboard component
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	// Enable tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// Initialize error handling
	CurrentErrorState = EAIErrorType::None;
	LastErrorMessage = TEXT("");
	TimeSinceLastRecoveryAttempt = 0.0f;
	
	// Initialize stuck detection
	PreviousLocation = FVector::ZeroVector;
	StuckTime = 0.0f;
	LastValidLocation = FVector::ZeroVector;
	
	// Initialize state tracking
	TimeInCurrentState = 0.0f;
	LastState = EAIState::Exploring;
}

void AMazeBlazeAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup perception system
	SetupPerceptionSystem();
}

void AMazeBlazeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// Reset error handling state
	CurrentErrorState = EAIErrorType::None;
	LastErrorMessage = TEXT("");
	TimeSinceLastRecoveryAttempt = 0.0f;
	StuckTime = 0.0f;
	TimeInCurrentState = 0.0f;
	
	// Initialize location tracking
	if (InPawn)
	{
		PreviousLocation = InPawn->GetActorLocation();
		LastValidLocation = InPawn->GetActorLocation();
	}
	
	// Initialize blackboard
	if (!BlackboardAsset || !BehaviorTreeAsset)
	{
		ReportAIError(EAIErrorType::AssetMissing, TEXT("Blackboard or Behavior Tree asset not set!"));
		return;
	}
	
	// Use the blackboard
	if (!UseBlackboard(BlackboardAsset, BlackboardComponent))
	{
		ReportAIError(EAIErrorType::BlackboardInitFailed, TEXT("Failed to initialize blackboard!"));
		return;
	}
	
	// Set self actor reference
	BlackboardComponent->SetValueAsObject(SelfActorKey, InPawn);
	
	// Set initial state to exploring
	SetCurrentState(EAIState::Exploring);
	LastState = EAIState::Exploring;
	
	// Run behavior tree
	if (!RunBehaviorTree(BehaviorTreeAsset))
	{
		ReportAIError(EAIErrorType::BehaviorTreeStartFailed, TEXT("Failed to run behavior tree!"));
		return;
	}
	
	// Log successful initialization
	UE_LOG(LogTemp, Log, TEXT("AI Controller successfully initialized for %s"), 
		   *InPawn->GetName());
	
	// Draw initial debug info
	DrawDebugInfo(5.0f);
}

void AMazeBlazeAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	// Stop behavior tree
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AMazeBlazeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Try to recover from errors
	if (IsInErrorState())
	{
		// Only try to recover every few seconds to avoid spamming recovery attempts
		TimeSinceLastRecoveryAttempt += DeltaTime;
		
		if (TimeSinceLastRecoveryAttempt >= RecoveryAttemptInterval)
		{
			TryRecoverFromError();
			TimeSinceLastRecoveryAttempt = 0.0f;
		}
	}
	else
	{
		// Check if AI is stuck
		if (IsAIStuck())
		{
			StuckTime += DeltaTime;
			
			// If stuck for too long, report error and try to recover
			if (StuckTime > MaxStuckTime)
			{
				ReportAIError(EAIErrorType::NavigationMissing, TEXT("AI appears to be stuck"));
				ResetAIState();
				StuckTime = 0.0f;
			}
		}
		else
		{
			// Reset stuck timer if moving
			StuckTime = 0.0f;
			
			// Store current location as valid if we're moving normally
			StoreValidLocation();
		}
		
		// Track time in current state
		EAIState CurrentState = GetCurrentState();
		if (CurrentState == LastState)
		{
			TimeInCurrentState += DeltaTime;
			
			// If in the same state for too long, consider it an error
			if (TimeInCurrentState > MaxTimeInState)
			{
				ReportAIError(EAIErrorType::TaskExecutionFailed, 
					FString::Printf(TEXT("Stuck in state %s for too long"), 
					*UEnum::GetValueAsString(CurrentState)));
				ResetAIState();
			}
		}
		else
		{
			// Reset timer when state changes
			TimeInCurrentState = 0.0f;
			LastState = CurrentState;
		}
		
		// Update perception data every tick when not in error state
		UpdatePerception();
		
		// Draw debug information
		DrawDebugInfo();
	}
}

EAIState AMazeBlazeAIController::GetCurrentState() const
{
	if (BlackboardComponent)
	{
		const uint8 StateValue = BlackboardComponent->GetValueAsEnum(CurrentStateKey);
		return static_cast<EAIState>(StateValue);
	}
	
	return EAIState::Exploring;
}

void AMazeBlazeAIController::SetCurrentState(EAIState NewState)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsEnum(CurrentStateKey, static_cast<uint8>(NewState));
	}
}

AMazeBlazeKey* AMazeBlazeAIController::FindNearestKey()
{
	TArray<AActor*> FoundKeys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeBlazeKey::StaticClass(), FoundKeys);
	
	if (FoundKeys.Num() == 0)
	{
		return nullptr;
	}
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return nullptr;
	}
	
	const FVector PawnLocation = ControlledPawn->GetActorLocation();
	AMazeBlazeKey* NearestKey = nullptr;
	float NearestDistance = MAX_FLT;
	
	for (AActor* Actor : FoundKeys)
	{
		AMazeBlazeKey* Key = Cast<AMazeBlazeKey>(Actor);
		if (Key)
		{
			// Skip keys that are not on the ground (already picked up)
			AMazeBlazeCharacter* MazeCharacter = Cast<AMazeBlazeCharacter>(ControlledPawn);
			if (MazeCharacter && MazeCharacter->GetCarriedKey() == Key)
			{
				continue;
			}
			
			const float Distance = FVector::Dist(PawnLocation, Key->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestKey = Key;
			}
		}
	}
	
	return NearestKey;
}

AMazeGameDoor* AMazeBlazeAIController::FindMatchingDoor(AMazeBlazeKey* Key)
{
	if (!Key)
	{
		return nullptr;
	}
	
	TArray<AActor*> FoundDoors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeGameDoor::StaticClass(), FoundDoors);
	
	if (FoundDoors.Num() == 0)
	{
		return nullptr;
	}
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return nullptr;
	}
	
	const FVector PawnLocation = ControlledPawn->GetActorLocation();
	AMazeGameDoor* NearestMatchingDoor = nullptr;
	float NearestDistance = MAX_FLT;
	
	for (AActor* Actor : FoundDoors)
	{
		AMazeGameDoor* Door = Cast<AMazeGameDoor>(Actor);
		if (Door && !Door->IsOpen() && Door->CanBeOpenedByKey(Key))
		{
			const float Distance = FVector::Dist(PawnLocation, Door->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestMatchingDoor = Door;
			}
		}
	}
	
	return NearestMatchingDoor;
}

AMazeBlazeExit* AMazeBlazeAIController::FindExit()
{
	TArray<AActor*> FoundExits;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMazeBlazeExit::StaticClass(), FoundExits);
	
	if (FoundExits.Num() == 0)
	{
		return nullptr;
	}
	
	// Just return the first exit found
	return Cast<AMazeBlazeExit>(FoundExits[0]);
}

void AMazeBlazeAIController::UpdatePerception()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		ReportAIError(EAIErrorType::TaskExecutionFailed, TEXT("No controlled pawn in UpdatePerception"));
		return;
	}
	
	if (!BlackboardComponent)
	{
		ReportAIError(EAIErrorType::BlackboardInitFailed, TEXT("No blackboard component in UpdatePerception"));
		return;
	}
	
	AMazeBlazeCharacter* MazeCharacter = Cast<AMazeBlazeCharacter>(ControlledPawn);
	if (!MazeCharacter)
	{
		ReportAIError(EAIErrorType::TaskExecutionFailed, TEXT("Controlled pawn is not a MazeBlazeCharacter"));
		return;
	}
	
	try
	{
		// Update current key
		AMazeBlazeKey* CarriedKey = MazeCharacter->GetCarriedKey();
		BlackboardComponent->SetValueAsObject(CurrentKeyKey, CarriedKey);
		
		// Find nearest key if not carrying one
		if (!CarriedKey)
		{
			AMazeBlazeKey* NearestKey = FindNearestKey();
			BlackboardComponent->SetValueAsObject(VisibleKeysKey, NearestKey);
			
			if (NearestKey)
			{
				// If we see a key and we're exploring, switch to seeking key
				if (GetCurrentState() == EAIState::Exploring)
				{
					SetCurrentState(EAIState::SeekingKey);
					BlackboardComponent->SetValueAsVector(CurrentTargetKey, NearestKey->GetActorLocation());
				}
			}
		}
		else
		{
			// If carrying a key, find a matching door
			AMazeGameDoor* MatchingDoor = FindMatchingDoor(CarriedKey);
			BlackboardComponent->SetValueAsObject(VisibleDoorsKey, MatchingDoor);
			
			if (MatchingDoor)
			{
				// If we see a matching door, switch to seeking door
				SetCurrentState(EAIState::SeekingDoor);
				BlackboardComponent->SetValueAsVector(CurrentTargetKey, MatchingDoor->GetActorLocation());
			}
		}
		
		// Check for exit
		AMazeBlazeExit* Exit = FindExit();
		if (Exit)
		{
			BlackboardComponent->SetValueAsVector(ExitLocationKey, Exit->GetActorLocation());
			
			// If we're not carrying a key and there are no visible keys, go to exit
			if (!CarriedKey && !BlackboardComponent->GetValueAsObject(VisibleKeysKey))
			{
				SetCurrentState(EAIState::GoingToExit);
				BlackboardComponent->SetValueAsVector(CurrentTargetKey, Exit->GetActorLocation());
			}
		}
		
		// If we got here without errors, clear any perception errors
		if (CurrentErrorState == EAIErrorType::PerceptionError)
		{
			CurrentErrorState = EAIErrorType::None;
			LastErrorMessage = TEXT("");
		}
	}
	catch (const std::exception& e)
	{
		// Handle any unexpected exceptions
		ReportAIError(EAIErrorType::PerceptionError, 
			FString::Printf(TEXT("Exception in UpdatePerception: %s"), UTF8_TO_TCHAR(e.what())));
	}
	catch (...)
	{
		// Handle any other unexpected errors
		ReportAIError(EAIErrorType::PerceptionError, TEXT("Unknown error in UpdatePerception"));
	}
}

void AMazeBlazeAIController::InteractWithObject(AActor* InteractableObject)
{
	if (!InteractableObject)
	{
		return;
	}
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	
	AMazeBlazeCharacter* MazeCharacter = Cast<AMazeBlazeCharacter>(ControlledPawn);
	if (!MazeCharacter)
	{
		return;
	}
	
	// Check if the object implements the interactable interface
	IMazeBlazeInteractableInterface* Interactable = Cast<IMazeBlazeInteractableInterface>(InteractableObject);
	if (Interactable && Interactable->CanInteractWith_Implementation(MazeCharacter))
	{
		// Interact with the object
		Interactable->InteractWith_Implementation(MazeCharacter);
		
		// Update perception after interaction
		UpdatePerception();
	}
}

void AMazeBlazeAIController::SetupPerceptionSystem()
{
	// Create sight config
	UAISenseConfig_Sight* SightConfig = NewObject<UAISenseConfig_Sight>(this);
	if (SightConfig)
	{
		// Configure sight parameters
		SightConfig->SightRadius = 1500.0f;
		SightConfig->LoseSightRadius = 2000.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		// Add sight config to perception component
		PerceptionComponent->ConfigureSense(*SightConfig);
		PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}
	else
	{
		ReportAIError(EAIErrorType::PerceptionError, TEXT("Failed to create sight configuration"));
	}
}

void AMazeBlazeAIController::ReportAIError(EAIErrorType ErrorType, const FString& ErrorMessage)
{
	// Store error information
	CurrentErrorState = ErrorType;
	LastErrorMessage = ErrorMessage;
	
	// Log the error
	UE_LOG(LogTemp, Error, TEXT("AI Error: %s - %s"), 
		   *UEnum::GetValueAsString(CurrentErrorState), *ErrorMessage);
	
	// Visual debugging
	if (GetPawn())
	{
		// Draw debug sphere at AI location to indicate error
		DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 100.0f, 12, 
					FColor::Red, false, 5.0f, 0, 2.0f);
		
		// Draw debug string above the AI
		DrawDebugString(GetWorld(), GetPawn()->GetActorLocation() + FVector(0, 0, 100), 
					*UEnum::GetValueAsString(CurrentErrorState),
					nullptr, FColor::Red, 5.0f, true);
	}
}

bool AMazeBlazeAIController::TryRecoverFromError()
{
	bool bRecovered = false;
	
	// Log recovery attempt
	UE_LOG(LogTemp, Warning, TEXT("Attempting to recover from error: %s - %s"), 
		   *UEnum::GetValueAsString(CurrentErrorState), *LastErrorMessage);
	
	// Reset stuck detection timers regardless of error type
	StuckTime = 0.0f;
	TimeInCurrentState = 0.0f;
	
	switch (CurrentErrorState)
	{
		case EAIErrorType::AssetMissing:
			// Can't recover from missing assets at runtime
			UE_LOG(LogTemp, Warning, TEXT("Cannot recover from missing assets at runtime"));
			break;
			
		case EAIErrorType::BlackboardInitFailed:
			// Try to reinitialize blackboard
			if (BlackboardAsset)
			{
				bRecovered = UseBlackboard(BlackboardAsset, BlackboardComponent);
				if (bRecovered && GetPawn())
				{
					// Reset blackboard values
					BlackboardComponent->SetValueAsObject(SelfActorKey, GetPawn());
					SetCurrentState(EAIState::Exploring);
				}
			}
			break;
			
		case EAIErrorType::BehaviorTreeStartFailed:
			// Try to restart behavior tree
			if (BehaviorTreeAsset)
			{
				bRecovered = RunBehaviorTree(BehaviorTreeAsset);
			}
			break;
			
		case EAIErrorType::NavigationMissing:
			// First try to use last valid location if available
			if (!LastValidLocation.IsZero() && GetPawn())
			{
				// Stop current movement
				StopMovement();
				
				// Set target to last valid location
				if (BlackboardComponent)
				{
					BlackboardComponent->SetValueAsVector(CurrentTargetKey, LastValidLocation);
				}
				
				// Move with a larger acceptance radius for recovery
				EPathFollowingRequestResult::Type MoveResult = MoveToLocation(LastValidLocation, 200.0f);
				bRecovered = (MoveResult != EPathFollowingRequestResult::Failed);
				
				UE_LOG(LogTemp, Warning, TEXT("Recovery: Moving to last valid location: %s"), 
					   bRecovered ? TEXT("Success") : TEXT("Failed"));
			}
			
			// If that fails or no valid location, try random point
			if (!bRecovered && GetPawn())
			{
				FNavLocation NavLocation;
				UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
				
				// Try with increasing radius until we find a valid point
				for (float Radius = 500.0f; Radius <= 3000.0f; Radius *= 2.0f)
				{
					if (NavSys && NavSys->GetRandomReachablePointInRadius(
						GetPawn()->GetActorLocation(), Radius, NavLocation))
					{
						// Set a new exploration target
						if (BlackboardComponent)
						{
							BlackboardComponent->SetValueAsVector(CurrentTargetKey, NavLocation.Location);
							SetCurrentState(EAIState::Exploring);
						}
						
						// Try to move to the new location
						EPathFollowingRequestResult::Type MoveResult = MoveToLocation(NavLocation.Location, 200.0f);
						bRecovered = (MoveResult != EPathFollowingRequestResult::Failed);
						
						UE_LOG(LogTemp, Warning, TEXT("Recovery: Moving to random point (radius %.0f): %s"), 
							   Radius, bRecovered ? TEXT("Success") : TEXT("Failed"));
						
						if (bRecovered)
						{
							break; // Exit the loop if we found a valid point
						}
					}
				}
			}
			break;
			
		case EAIErrorType::PerceptionError:
			// Try to reset perception system
			if (PerceptionComponent)
			{
				PerceptionComponent->ForgetAll();
				SetupPerceptionSystem();
				
				// Force update perception
				UpdatePerception();
				
				bRecovered = true;
			}
			break;
			
		case EAIErrorType::TaskExecutionFailed:
			// Use ResetAIState for comprehensive recovery
			ResetAIState();
			bRecovered = true;
			break;
			
		case EAIErrorType::None:
			// No error to recover from
			bRecovered = true;
			break;
	}
	
	if (bRecovered)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI recovered from error: %s"), 
			   *UEnum::GetValueAsString(CurrentErrorState));
		CurrentErrorState = EAIErrorType::None;
		LastErrorMessage = TEXT("");
		
		// Visual feedback for recovery
		if (GetPawn())
		{
			DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 100.0f, 12, 
						FColor::Green, false, 2.0f, 0, 2.0f);
			
			DrawDebugString(GetWorld(), GetPawn()->GetActorLocation() + FVector(0, 0, 150), 
						TEXT("RECOVERED"), nullptr, FColor::Green, 3.0f, true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI failed to recover from error: %s"), 
			   *UEnum::GetValueAsString(CurrentErrorState));
		
		// Visual feedback for failed recovery
		if (GetPawn())
		{
			DrawDebugSphere(GetWorld(), GetPawn()->GetActorLocation(), 120.0f, 12, 
						FColor::Red, false, 2.0f, 0, 3.0f);
			
			DrawDebugString(GetWorld(), GetPawn()->GetActorLocation() + FVector(0, 0, 150), 
						TEXT("RECOVERY FAILED"), nullptr, FColor::Red, 3.0f, true);
		}
	}
	
	return bRecovered;
}

FString AMazeBlazeAIController::GetDebugStatusText() const
{
	FString StatusText;
	
	// Add current state
	StatusText += FString::Printf(TEXT("State: %s (%.1fs)\n"), 
					*UEnum::GetValueAsString(GetCurrentState()),
					TimeInCurrentState);
	
	// Add current target if available
	if (BlackboardComponent)
	{
		FVector CurrentTarget = BlackboardComponent->GetValueAsVector(CurrentTargetKey);
		if (!CurrentTarget.IsZero())
		{
			StatusText += FString::Printf(TEXT("Target: (%.0f, %.0f, %.0f)\n"), 
						CurrentTarget.X, CurrentTarget.Y, CurrentTarget.Z);
		}
		
		// Add key status
		UObject* CurrentKey = BlackboardComponent->GetValueAsObject(CurrentKeyKey);
		StatusText += FString::Printf(TEXT("Has Key: %s\n"), 
					CurrentKey ? TEXT("Yes") : TEXT("No"));
	}
	
	// Add stuck information
	if (StuckTime > 0.0f)
	{
		StatusText += FString::Printf(TEXT("Stuck: %.1fs / %.1fs\n"), 
					StuckTime, MaxStuckTime);
	}
	
	// Add error information if any
	if (IsInErrorState())
	{
		StatusText += FString::Printf(TEXT("ERROR: %s\n%s"), 
					*UEnum::GetValueAsString(CurrentErrorState),
					*LastErrorMessage);
	}
	
	return StatusText;
}

bool AMazeBlazeAIController::IsAIStuck()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}
	
	FVector CurrentLocation = ControlledPawn->GetActorLocation();
	
	// If this is the first check, initialize previous location
	if (PreviousLocation.IsZero())
	{
		PreviousLocation = CurrentLocation;
		return false;
	}
	
	// Check if the AI has moved significantly
	bool bIsStuck = FVector::DistSquared(CurrentLocation, PreviousLocation) < (StuckThreshold * StuckThreshold);
	
	// Only consider stuck if we're trying to move
	if (bIsStuck && GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving)
	{
		// We're stuck
		return true;
	}
	
	// Update previous location for next check
	PreviousLocation = CurrentLocation;
	return false;
}

void AMazeBlazeAIController::ResetAIState()
{
	// Stop current movement
	StopMovement();
	
	// Reset timers
	StuckTime = 0.0f;
	TimeInCurrentState = 0.0f;
	
	// Reset state to exploring
	SetCurrentState(EAIState::Exploring);
	
	// Try to find a new navigation point
	if (GetPawn() && !LastValidLocation.IsZero())
	{
		// First try to move back to last valid location
		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsVector(CurrentTargetKey, LastValidLocation);
			MoveToLocation(LastValidLocation, 200.0f); // Larger acceptance radius for recovery
		}
	}
	else
	{
		// If no valid location stored, try to find a random point
		FNavLocation NavLocation;
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys && GetPawn())
		{
			if (NavSys->GetRandomReachablePointInRadius(
				GetPawn()->GetActorLocation(), 1000.0f, NavLocation))
			{
				if (BlackboardComponent)
				{
					BlackboardComponent->SetValueAsVector(CurrentTargetKey, NavLocation.Location);
				}
				MoveToLocation(NavLocation.Location, 200.0f);
			}
		}
	}
	
	// If we have a behavior tree component, restart it
	if (BehaviorTreeComponent && BehaviorTreeAsset)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void AMazeBlazeAIController::DrawDebugInfo(float Duration)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || !GetWorld())
	{
		return;
	}
	
	FVector PawnLocation = ControlledPawn->GetActorLocation();
	FVector PawnForward = ControlledPawn->GetActorForwardVector();
	
	// Draw current state
	FString StateText = UEnum::GetValueAsString(GetCurrentState());
	DrawDebugString(GetWorld(), PawnLocation + FVector(0, 0, 100), *StateText,
		nullptr, FColor::White, Duration);
	
	// Draw path to target if available
	if (BlackboardComponent)
	{
		FVector TargetLocation = BlackboardComponent->GetValueAsVector(CurrentTargetKey);
		if (!TargetLocation.IsZero())
		{
			// Draw line to target
			DrawDebugLine(GetWorld(), PawnLocation, TargetLocation, 
				FColor::Green, false, Duration, 0, 2.0f);
			
			// Draw sphere at target
			DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, 
				FColor::Green, false, Duration);
		}
	}
	
	// Draw last valid location if available
	if (!LastValidLocation.IsZero())
	{
		DrawDebugSphere(GetWorld(), LastValidLocation, 30.0f, 8, 
			FColor::Blue, false, Duration);
	}
	
	// Draw stuck indicator if stuck
	if (StuckTime > 0.0f)
	{
		// Size increases with stuck time
		float Size = FMath::Lerp(50.0f, 100.0f, FMath::Min(StuckTime / MaxStuckTime, 1.0f));
		DrawDebugSphere(GetWorld(), PawnLocation, Size, 12, 
			FColor::Yellow, false, Duration);
	}
	
	// Draw perception cone
	if (PerceptionComponent)
	{
		const float SightRadius = 1500.0f; // Match the sight radius from SetupPerceptionSystem
		const float HalfAngle = 45.0f; // Half of PeripheralVisionAngleDegrees
		
		// Draw sight cone
		DrawDebugCone(GetWorld(), PawnLocation, PawnForward, SightRadius,
			FMath::DegreesToRadians(HalfAngle), FMath::DegreesToRadians(HalfAngle),
			12, FColor::Cyan, false, Duration);
	}
}

void AMazeBlazeAIController::StoreValidLocation()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	
	// Only store location if we're not in an error state and not stuck
	if (!IsInErrorState() && StuckTime <= 0.0f)
	{
		LastValidLocation = ControlledPawn->GetActorLocation();
	}
}