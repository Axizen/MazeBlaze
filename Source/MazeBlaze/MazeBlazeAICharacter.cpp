// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeBlazeAICharacter.h"
#include "MazeBlazeKey.h"
#include "MazeGameDoor.h"
#include "MazeBlazeExit.h"
#include "MazeBlazeGameInstance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMazeBlazeAICharacter::AMazeBlazeAICharacter()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Initialize metrics
	KeysCollected = 0;
	DoorsOpened = 0;
	BacktrackingInstances = 0;
	TimeTaken = 0.0f;
	bIsControlledByAI = true;
	bReachedExit = false;

	// Create a static mesh component for visualizing AI path (optional)
	AIVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIVisualMesh"));
	if (AIVisualMesh)
	{
		AIVisualMesh->SetupAttachment(RootComponent);
		AIVisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AIVisualMesh->SetVisibility(false); // Hidden by default, enable for debugging
	}
}

// Called when the game starts or when spawned
void AMazeBlazeAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize AI character
	InitializeAICharacter();

	// Bind to the key pickup event
	OnPickupKey.AddDynamic(this, &AMazeBlazeAICharacter::OnKeyCollected);
}

// Called every frame
void AMazeBlazeAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update time taken if the AI is active and hasn't reached the exit
	if (bIsControlledByAI && !bReachedExit)
	{
		TimeTaken += DeltaTime;
	}
}

void AMazeBlazeAICharacter::InitializeAICharacter()
{
	// Get the game instance to check AI settings
	UMazeBlazeGameInstance* GameInstance = Cast<UMazeBlazeGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("MazeBlazeAICharacter: Failed to get game instance"));
		return;
	}

	// Check if we should play as AI
	if (GameInstance->GetPlayAsAI())
	{
		bIsControlledByAI = true;
		
		// Log the AI exploration system being used
		EAIExplorationSystem ExplorationSystem = GameInstance->GetAIExplorationSystem();
		UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Using exploration system: %d"), static_cast<int32>(ExplorationSystem));
		
		// Make sure we have an AI controller
		AAIController* AIController = Cast<AAIController>(GetController());
		if (!AIController)
		{
			UE_LOG(LogTemp, Warning, TEXT("MazeBlazeAICharacter: No AI controller found"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: AI controller found"));
		}
	}
	else
	{
		bIsControlledByAI = false;
		UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Not controlled by AI"));
	}
}

void AMazeBlazeAICharacter::OnKeyCollected(AMazeBlazeKey* Key)
{
	if (!Key)
	{
		return;
	}

	// Increment the keys collected counter
	KeysCollected++;

	// Log the key collection
	UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Collected key %s (Signature: %d)"), 
		*Key->GetKeyName().ToString(), Key->GetSignature());

	// Broadcast the event
	OnKeyPickedUp.Broadcast(Key);
}

void AMazeBlazeAICharacter::OnDoorOpened(AMazeGameDoor* Door)
{
	if (!Door)
	{
		return;
	}

	// Increment the doors opened counter
	DoorsOpened++;

	// Log the door opening
	UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Opened door at location %s"), 
		*Door->GetActorLocation().ToString());

	// Broadcast the event
	OnDoorOpenedEvent.Broadcast(Door);
}

void AMazeBlazeAICharacter::OnReachedExit(AMazeBlazeExit* Exit)
{
	if (!Exit)
	{
		return;
	}

	// Set the reached exit flag
	bReachedExit = true;

	// Log the exit reaching
	UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Reached exit after %.2f seconds"), TimeTaken);
	
	// Log performance metrics
	UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Performance Metrics - %s"), *GetPerformanceMetrics());

	// Broadcast the event
	OnExitReached.Broadcast(Exit);
}

FString AMazeBlazeAICharacter::GetPerformanceMetrics() const
{
	// Format the performance metrics as a string
	return FString::Printf(TEXT("Time: %.2f seconds, Keys Collected: %d, Doors Opened: %d, Backtracking Instances: %d"),
		TimeTaken, KeysCollected, DoorsOpened, BacktrackingInstances);
}

void AMazeBlazeAICharacter::UpdatePathVisualization(const TArray<FVector>& Path)
{
	// Clear any existing debug lines
	FlushPersistentDebugLines(GetWorld());

	// Draw the path as debug lines
	if (Path.Num() > 1)
	{
		for (int32 i = 0; i < Path.Num() - 1; i++)
		{
			DrawDebugLine(
				GetWorld(),
				Path[i],
				Path[i + 1],
				FColor::Green,
				true,
				-1.0f,
				0,
				2.0f
			);
		}
	}
}

bool AMazeBlazeAICharacter::DetectBacktracking(const FVector& NewLocation)
{
	// Simple backtracking detection - check if we're revisiting a location that's in our current path
	for (const FVector& PathPoint : CurrentPath)
	{
		// If we're close to a previous path point (excluding the most recent ones)
		if (FVector::Dist(NewLocation, PathPoint) < 100.0f)
		{
			BacktrackingInstances++;
			UE_LOG(LogTemp, Display, TEXT("MazeBlazeAICharacter: Backtracking detected at location %s"), 
				*NewLocation.ToString());
			return true;
		}
	}

	return false;
}

void AMazeBlazeAICharacter::SetCurrentPath(const TArray<FVector>& NewPath)
{
	// Update the current path
	CurrentPath = NewPath;

	// Visualize the path if debugging is enabled
	UpdatePathVisualization(CurrentPath);
}