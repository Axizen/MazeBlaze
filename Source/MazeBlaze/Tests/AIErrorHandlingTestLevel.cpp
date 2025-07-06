// AIErrorHandlingTestLevel.cpp
// Helper class for setting up a test level for AI error handling tests

#include "AIErrorHandlingTestLevel.h"
#include "AIErrorHandlingTests.h"
#include "AIErrorHandlingTestCommands.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"
#include "Engine/TriggerVolume.h"
#include "DrawDebugHelpers.h"

// Include our AI controller and related classes
#include "../MazeBlazeAIController.h"
#include "../MazeBlazeAICharacter.h"
#include "../MazeBlazeKey.h"
#include "../MazeGameDoor.h"
#include "../MazeBlazeExit.h"

AAIErrorHandlingTestLevel::AAIErrorHandlingTestLevel()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // Default values
    bAutoRunTestsOnBeginPlay = false;
    AutoRunTestsDelay = 5.0f;
}

void AAIErrorHandlingTestLevel::BeginPlay()
{
    Super::BeginPlay();
    
    // Auto-run tests if enabled
    if (bAutoRunTestsOnBeginPlay)
    {
        GetWorldTimerManager().SetTimer(AutoRunTestsTimerHandle, this, &AAIErrorHandlingTestLevel::AutoRunTests, AutoRunTestsDelay, false);
        UE_LOG(LogTemp, Display, TEXT("AI Error Handling tests will auto-run in %.1f seconds"), AutoRunTestsDelay);
    }
}

void AAIErrorHandlingTestLevel::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAIErrorHandlingTestLevel::SpawnTestAICharacters(int32 Count)
{
    if (!AICharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot spawn test AI characters: AICharacterClass is not set"));
        return;
    }
    
    // Clear any existing test characters
    for (AMazeBlazeAICharacter* Character : TestAICharacters)
    {
        if (Character)
        {
            Character->Destroy();
        }
    }
    TestAICharacters.Empty();
    
    // Get the navigation system
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot spawn test AI characters: Navigation system not found"));
        return;
    }
    
    // Spawn the requested number of characters
    for (int32 i = 0; i < Count; ++i)
    {
        // Find a random navigable point
        FNavLocation SpawnLocation;
        if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), 1000.0f, SpawnLocation))
        {
            // Spawn the character
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            
            AMazeBlazeAICharacter* Character = GetWorld()->SpawnActor<AMazeBlazeAICharacter>(
                AICharacterClass, 
                SpawnLocation.Location + FVector(0, 0, 100.0f), // Raise slightly above ground
                FRotator::ZeroRotator,
                SpawnParams);
            
            if (Character)
            {
                // Configure the AI controller
                AMazeBlazeAIController* AIController = Cast<AMazeBlazeAIController>(Character->GetController());
                if (AIController)
                {
                    // Set test behavior tree and blackboard if provided
                    if (TestBehaviorTree)
                    {
                        AIController->BehaviorTreeAsset = TestBehaviorTree;
                    }
                    
                    if (TestBlackboard)
                    {
                        AIController->BlackboardAsset = TestBlackboard;
                    }
                    
                    // Restart the AI controller to apply changes
                    AIController->UnPossess();
                    AIController->Possess(Character);
                }
                
                TestAICharacters.Add(Character);
                
                // Draw debug sphere to mark spawn location
                DrawDebugSphere(GetWorld(), SpawnLocation.Location, 50.0f, 12, FColor::Green, false, 10.0f);
            }
        }
    }
    
    UE_LOG(LogTemp, Display, TEXT("Spawned %d test AI characters"), TestAICharacters.Num());
}

void AAIErrorHandlingTestLevel::CreateTestZones()
{
    // Clear any existing test zones
    for (AActor* Zone : TestZones)
    {
        if (Zone)
        {
            Zone->Destroy();
        }
    }
    TestZones.Empty();
    
    // Create test zones at different locations around the level
    CreateNavigationErrorZone(GetActorLocation() + FVector(1000.0f, 0.0f, 0.0f));
    CreateTaskExecutionErrorZone(GetActorLocation() + FVector(0.0f, 1000.0f, 0.0f));
    CreatePerceptionErrorZone(GetActorLocation() + FVector(-1000.0f, 0.0f, 0.0f));
    
    UE_LOG(LogTemp, Display, TEXT("Created %d test zones"), TestZones.Num());
}

void AAIErrorHandlingTestLevel::TriggerErrorOnAllAI(const FString& ErrorType)
{
    // Find all AI controllers in the world
    TArray<AMazeBlazeAIController*> AIControllers;
    for (TActorIterator<AMazeBlazeAIController> It(GetWorld()); It; ++It)
    {
        AIControllers.Add(*It);
    }
    
    if (AIControllers.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No AI controllers found to trigger error on"));
        return;
    }
    
    // Determine which error type to trigger
    EAIErrorType ErrorTypeEnum = EAIErrorType::None;
    FString ErrorMessage = TEXT("Manually triggered error for testing");
    
    if (ErrorType.Equals(TEXT("AssetMissing"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::AssetMissing;
    }
    else if (ErrorType.Equals(TEXT("BlackboardInitFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::BlackboardInitFailed;
    }
    else if (ErrorType.Equals(TEXT("BehaviorTreeStartFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::BehaviorTreeStartFailed;
    }
    else if (ErrorType.Equals(TEXT("NavigationMissing"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::NavigationMissing;
    }
    else if (ErrorType.Equals(TEXT("PerceptionError"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::PerceptionError;
    }
    else if (ErrorType.Equals(TEXT("TaskExecutionFailed"), ESearchCase::IgnoreCase))
    {
        ErrorTypeEnum = EAIErrorType::TaskExecutionFailed;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown error type: %s"), *ErrorType);
        return;
    }
    
    // Trigger the error on all AI controllers
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController)
        {
            AIController->ReportAIError(ErrorTypeEnum, ErrorMessage);
        }
    }
    
    UE_LOG(LogTemp, Display, TEXT("Triggered %s error on %d AI controllers"), *ErrorType, AIControllers.Num());
}

void AAIErrorHandlingTestLevel::ResetAllAI()
{
    // Find all AI controllers in the world
    TArray<AMazeBlazeAIController*> AIControllers;
    for (TActorIterator<AMazeBlazeAIController> It(GetWorld()); It; ++It)
    {
        AIControllers.Add(*It);
    }
    
    if (AIControllers.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No AI controllers found to reset"));
        return;
    }
    
    // Reset all AI controllers
    for (AMazeBlazeAIController* AIController : AIControllers)
    {
        if (AIController)
        {
            // Get the controlled pawn
            APawn* ControlledPawn = AIController->GetPawn();
            if (ControlledPawn)
            {
                // Reset error state
                AIController->CurrentErrorState = EAIErrorType::None;
                AIController->LastErrorMessage = TEXT("");
                
                // Restart the AI controller
                AIController->UnPossess();
                AIController->Possess(ControlledPawn);
            }
        }
    }
    
    UE_LOG(LogTemp, Display, TEXT("Reset %d AI controllers"), AIControllers.Num());
}

void AAIErrorHandlingTestLevel::CreateNavigationErrorZone(const FVector& Location)
{
    // Create a trigger volume
    ATriggerVolume* TriggerVolume = GetWorld()->SpawnActor<ATriggerVolume>(
        ATriggerVolume::StaticClass(),
        Location,
        FRotator::ZeroRotator);
    
    if (TriggerVolume)
    {
        // Set up the trigger volume
        TriggerVolume->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        TriggerVolume->SetActorLabel(TEXT("NavigationErrorZone"));
        
        // Add a text render component
        UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(TriggerVolume);
        if (TextComponent)
        {
            TextComponent->RegisterComponent();
            TextComponent->AttachToComponent(TriggerVolume->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
            TextComponent->SetText(FText::FromString(TEXT("Navigation Error Test Zone")));
            TextComponent->SetTextRenderColor(FColor::Red);
            TextComponent->SetWorldSize(150.0f);
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
        }
        
        // Draw debug box to mark the zone
        DrawDebugBox(GetWorld(), Location, TriggerVolume->GetComponentsBoundingBox().GetExtent(), FColor::Red, false, 100.0f);
        
        TestZones.Add(TriggerVolume);
    }
}

void AAIErrorHandlingTestLevel::CreateTaskExecutionErrorZone(const FVector& Location)
{
    // Create a trigger volume
    ATriggerVolume* TriggerVolume = GetWorld()->SpawnActor<ATriggerVolume>(
        ATriggerVolume::StaticClass(),
        Location,
        FRotator::ZeroRotator);
    
    if (TriggerVolume)
    {
        // Set up the trigger volume
        TriggerVolume->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        TriggerVolume->SetActorLabel(TEXT("TaskExecutionErrorZone"));
        
        // Add a text render component
        UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(TriggerVolume);
        if (TextComponent)
        {
            TextComponent->RegisterComponent();
            TextComponent->AttachToComponent(TriggerVolume->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
            TextComponent->SetText(FText::FromString(TEXT("Task Execution Error Test Zone")));
            TextComponent->SetTextRenderColor(FColor::Yellow);
            TextComponent->SetWorldSize(150.0f);
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
        }
        
        // Draw debug box to mark the zone
        DrawDebugBox(GetWorld(), Location, TriggerVolume->GetComponentsBoundingBox().GetExtent(), FColor::Yellow, false, 100.0f);
        
        TestZones.Add(TriggerVolume);
    }
}

void AAIErrorHandlingTestLevel::CreatePerceptionErrorZone(const FVector& Location)
{
    // Create a trigger volume
    ATriggerVolume* TriggerVolume = GetWorld()->SpawnActor<ATriggerVolume>(
        ATriggerVolume::StaticClass(),
        Location,
        FRotator::ZeroRotator);
    
    if (TriggerVolume)
    {
        // Set up the trigger volume
        TriggerVolume->SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
        TriggerVolume->SetActorLabel(TEXT("PerceptionErrorZone"));
        
        // Add a text render component
        UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(TriggerVolume);
        if (TextComponent)
        {
            TextComponent->RegisterComponent();
            TextComponent->AttachToComponent(TriggerVolume->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            TextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
            TextComponent->SetText(FText::FromString(TEXT("Perception Error Test Zone")));
            TextComponent->SetTextRenderColor(FColor::Blue);
            TextComponent->SetWorldSize(150.0f);
            TextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
        }
        
        // Draw debug box to mark the zone
        DrawDebugBox(GetWorld(), Location, TriggerVolume->GetComponentsBoundingBox().GetExtent(), FColor::Blue, false, 100.0f);
        
        TestZones.Add(TriggerVolume);
    }
}

void AAIErrorHandlingTestLevel::AutoRunTests()
{
    UE_LOG(LogTemp, Display, TEXT("Auto-running AI Error Handling tests..."));
    
    // Run all tests
    RunAIErrorHandlingTests(GetWorld());
}