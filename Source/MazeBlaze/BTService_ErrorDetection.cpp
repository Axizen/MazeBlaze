// BTService_ErrorDetection.cpp
// Behavior Tree service that monitors for AI errors

#include "BTService_ErrorDetection.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"

UBTService_ErrorDetection::UBTService_ErrorDetection()
{
    NodeName = TEXT("Error Detection");
    Interval = 1.0f;
    RandomDeviation = 0.1f;
}

void UBTService_ErrorDetection::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return;
    }
    
    AMazeBlazeAIController* MazeAIController = Cast<AMazeBlazeAIController>(AIController);
    if (!MazeAIController)
    {
        return;
    }
    
    // Skip error detection if already in error state
    if (MazeAIController->IsInErrorState())
    {
        return;
    }
    
    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                      TEXT("No controlled pawn"));
        return;
    }
    
    // Get current location
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    
    // Check for navigation errors
    CheckNavigationErrors(MazeAIController, CurrentLocation);
    
    // Check for state-specific errors
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        CheckStateErrors(MazeAIController, BlackboardComp);
    }
    
    // Draw debug information if enabled
    if (bDrawDebugInfo)
    {
        MazeAIController->DrawDebugInfo(Interval * 1.1f);
    }
}

FString UBTService_ErrorDetection::GetStaticDescription() const
{
    return FString::Printf(TEXT("Error Detection\nStuck Threshold: %.1f units\nMax Stuck Time: %.1f sec\nMax Path Time: %.1f sec"), 
                         StuckDistanceThreshold, MaxStuckTime, MaxPathFollowingTime);
}

bool UBTService_ErrorDetection::IsAIStuck(const AAIController* AIController, const FVector& CurrentLocation)
{
    // First call, initialize last location
    if (LastLocation.IsZero())
    {
        LastLocation = CurrentLocation;
        StuckTime = 0.0f;
        return false;
    }
    
    // Check if AI has moved
    float DistanceSquared = FVector::DistSquared(CurrentLocation, LastLocation);
    if (DistanceSquared < StuckDistanceThreshold * StuckDistanceThreshold)
    {
        // AI hasn't moved much, increment stuck time
        StuckTime += Interval;
        
        // Check if we're trying to move
        if (AIController->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving)
        {
            // If stuck for too long while trying to move, report stuck
            if (StuckTime >= MaxStuckTime)
            {
                return true;
            }
        }
        else
        {
            // If not trying to move, don't consider it stuck
            StuckTime = 0.0f;
        }
    }
    else
    {
        // AI is moving, reset stuck time
        StuckTime = 0.0f;
    }
    
    // Update last location
    LastLocation = CurrentLocation;
    
    return false;
}

void UBTService_ErrorDetection::CheckNavigationErrors(AMazeBlazeAIController* MazeAIController, const FVector& CurrentLocation)
{
    // Check if AI is stuck
    if (IsAIStuck(MazeAIController, CurrentLocation))
    {
        MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
                                      TEXT("AI appears to be stuck while moving"));
        return;
    }
    
    // Check path following status
    if (MazeAIController->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving)
    {
        PathFollowingTimer += Interval;
        
        // If path following takes too long, report error
        if (PathFollowingTimer > MaxPathFollowingTime)
        {
            MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
                                          TEXT("Path following taking too long"));
            
            // Reset timer
            PathFollowingTimer = 0.0f;
        }
    }
    else
    {
        // Reset timer if not moving
        PathFollowingTimer = 0.0f;
    }
    
    // Check if we're on a valid navigation mesh
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(MazeAIController->GetWorld());
    if (NavSys)
    {
        FNavLocation NavLocation;
        if (!NavSys->ProjectPointToNavigation(CurrentLocation, NavLocation, FVector(100.0f, 100.0f, 100.0f)))
        {
            MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
                                          TEXT("AI is not on a valid navigation mesh"));
        }
    }
}

void UBTService_ErrorDetection::CheckStateErrors(AMazeBlazeAIController* MazeAIController, UBlackboardComponent* BlackboardComp)
{
    // Get current state
    EAIState CurrentState = MazeAIController->GetCurrentState();
    
    // Check if state has changed
    if (CurrentState != LastState)
    {
        // Reset time in state
        TimeInState = 0.0f;
        LastState = CurrentState;
    }
    else
    {
        // Increment time in state
        TimeInState += Interval;
        
        // Check if we've been in the same state too long
        if (TimeInState > MaxTaskExecutionTime)
        {
            MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                          FString::Printf(TEXT("Stuck in state %s for too long"), 
                                                        *UEnum::GetValueAsString(CurrentState)));
            
            // Reset time in state
            TimeInState = 0.0f;
        }
    }
    
    // Check for state-specific errors
    switch (CurrentState)
    {
        case EAIState::SeekingKey:
        {
            // Check if we have a visible key
            if (!BlackboardComp->GetValueAsObject("VisibleKeys"))
            {
                // If we've been seeking a key for a while but don't see one, report error
                if (TimeInState > 5.0f)
                {
                    MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                                  TEXT("Seeking key but no key is visible"));
                }
            }
            break;
        }
            
        case EAIState::SeekingDoor:
        {
            // Check if we have a key and a visible door
            if (!BlackboardComp->GetValueAsObject("CurrentKey") ||
                !BlackboardComp->GetValueAsObject("VisibleDoors"))
            {
                // If we've been seeking a door for a while but don't have a key or don't see a door, report error
                if (TimeInState > 5.0f)
                {
                    MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                                  TEXT("Seeking door but no key or door is available"));
                }
            }
            break;
        }
            
        case EAIState::GoingToExit:
        {
            // Check if we have an exit location
            FVector ExitLocation = BlackboardComp->GetValueAsVector("ExitLocation");
            if (ExitLocation.IsZero())
            {
                MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                              TEXT("Going to exit but no exit location is set"));
            }
            break;
        }
            
        default:
            break;
    }
    
    // Check if current target is valid when in certain states
    if (CurrentState != EAIState::Exploring)
    {
        FVector CurrentTarget = BlackboardComp->GetValueAsVector("CurrentTarget");
        if (CurrentTarget.IsZero())
        {
            MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                          TEXT("Current target is invalid"));
        }
    }
}