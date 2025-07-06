# MazeBlaze AI Error Handling System

## Table of Contents

1. [Overview](#overview)
2. [Components](#components)
3. [Error Types](#error-types)
4. [Recovery Strategies](#recovery-strategies)
5. [Debugging Tools](#debugging-tools)
6. [Extending the System](#extending-the-system)
7. [Best Practices](#best-practices)

## Overview

The MazeBlaze AI Error Handling System is a comprehensive framework designed to detect, report, and recover from common AI errors in maze-based navigation. The system enhances the robustness of AI behavior by providing automatic error detection and recovery mechanisms, along with extensive debugging tools.

The system addresses common issues in AI navigation and decision-making, such as:
- Getting stuck in geometry
- Failed path finding
- Perception system failures
- Behavior tree execution errors
- State transition problems

By implementing this error handling system, the MazeBlaze AI becomes more resilient, self-correcting, and easier to debug during development.

## Components

The error handling system consists of several integrated components:

### 1. MazeBlazeAIController

The AI controller has been enhanced with error handling capabilities:

```cpp
// Error type enum in MazeBlazeAIController.h
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

// Error handling properties
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Debug")
EAIErrorType CurrentErrorState;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Debug")
FString LastErrorMessage;

UPROPERTY(EditDefaultsOnly, Category = "AI|Debug")
float RecoveryAttemptInterval = 5.0f;

// Time tracking for recovery attempts
float TimeSinceLastRecoveryAttempt = 0.0f;

// Last valid location for recovery
FVector LastValidLocation;
```

Key methods:
- `ReportAIError` - Reports and logs errors with visual debugging
- `TryRecoverFromError` - Attempts to recover from different error types
- `IsInErrorState` - Checks if the AI is currently in an error state
- `GetDebugStatusText` - Provides detailed status text for debugging

### 2. BTService_ErrorDetection

A behavior tree service that continuously monitors for errors:

```cpp
UCLASS()
class MAZEBLAZE_API UBTService_ErrorDetection : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_ErrorDetection();
    
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
    
protected:
    // Maximum time to wait for path following to succeed
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxPathFollowingTime = 10.0f;
    
    // Maximum time to wait for a task to complete
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxTaskExecutionTime = 15.0f;
    
    // Maximum time allowed to be stuck in one place
    UPROPERTY(EditAnywhere, Category = "Error Detection")
    float MaxStuckTime = 5.0f;
    
private:
    // Time tracking for path following
    float PathFollowingTimer = 0.0f;
    
    // Last known location for stuck detection
    FVector LastLocation = FVector::ZeroVector;
    
    // Time at same location
    float StuckTime = 0.0f;
};
```

This service detects:
- When the AI is stuck in one place
- Path following timeouts
- State transition issues
- Invalid blackboard values

### 3. Enhanced Behavior Tree

The behavior tree has been enhanced with error handling capabilities:

- Error recovery branch with highest priority
- Stuck detection and recovery
- State monitoring and correction
- Path following recovery

### 4. Debug HUD

A dedicated HUD for visualizing AI status and errors:

- Toggle with F3 key
- Shows AI states and errors
- Displays minimap with AI positions
- Visualizes state transitions
- Highlights error conditions

## Error Types

The system detects and handles the following error types:

### 1. AssetMissing

**Detection**: Occurs when required assets (behavior tree, blackboard) are missing.

**Example**:
```cpp
if (!BlackboardAsset || !BehaviorTreeAsset)
{
    ReportAIError(EAIErrorType::AssetMissing, 
                 TEXT("Blackboard or Behavior Tree asset not set!"));
    return;
}
```

### 2. BlackboardInitFailed

**Detection**: Occurs when the blackboard fails to initialize.

**Example**:
```cpp
if (!UseBlackboard(BlackboardAsset, BlackboardComponent))
{
    ReportAIError(EAIErrorType::BlackboardInitFailed, 
                 TEXT("Failed to initialize blackboard!"));
    return;
}
```

### 3. BehaviorTreeStartFailed

**Detection**: Occurs when the behavior tree fails to start.

**Example**:
```cpp
if (!RunBehaviorTree(BehaviorTreeAsset))
{
    ReportAIError(EAIErrorType::BehaviorTreeStartFailed, 
                 TEXT("Failed to run behavior tree!"));
    return;
}
```

### 4. NavigationMissing

**Detection**: Occurs when navigation fails or the AI gets stuck.

**Example**:
```cpp
// In BTService_ErrorDetection
if (FVector::DistSquared(CurrentLocation, LastLocation) < 100.0f)
{
    StuckTime += DeltaSeconds;
    
    if (StuckTime > MaxStuckTime)
    {
        if (AIController->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving)
        {
            MazeAIController->ReportAIError(EAIErrorType::NavigationMissing, 
                                          TEXT("AI appears to be stuck"));
            StuckTime = 0.0f;
        }
    }
}
```

### 5. PerceptionError

**Detection**: Occurs when the perception system fails.

**Example**:
```cpp
try
{
    // Perception update code
}
catch (const std::exception& e)
{
    ReportAIError(EAIErrorType::PerceptionError, 
                 FString::Printf(TEXT("Perception error: %s"), UTF8_TO_TCHAR(e.what())));
}
```

### 6. TaskExecutionFailed

**Detection**: Occurs when a behavior tree task fails to execute.

**Example**:
```cpp
EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(NavLocation.Location);

if (MoveResult == EPathFollowingRequestResult::Failed)
{
    MazeAIController->ReportAIError(EAIErrorType::TaskExecutionFailed, 
                                  TEXT("Failed to start movement"));
    return EBTNodeResult::Failed;
}
```

## Recovery Strategies

The system implements different recovery strategies for each error type:

### 1. Asset Missing Recovery

This error cannot be recovered from at runtime. It requires fixing in the editor.

### 2. Blackboard Initialization Recovery

```cpp
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
```

### 3. Behavior Tree Start Recovery

```cpp
case EAIErrorType::BehaviorTreeStartFailed:
    // Try to restart behavior tree
    if (BehaviorTreeAsset)
    {
        bRecovered = RunBehaviorTree(BehaviorTreeAsset);
    }
    break;
```

### 4. Navigation Recovery

```cpp
case EAIErrorType::NavigationMissing:
    // Try to find a valid navigation point nearby
    if (GetPawn())
    {
        FNavLocation NavLocation;
        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSys && NavSys->GetRandomReachablePointInRadius(
            GetPawn()->GetActorLocation(), 1000.0f, NavLocation))
        {
            // Set a new exploration target
            if (BlackboardComponent)
            {
                BlackboardComponent->SetValueAsVector(CurrentTargetKey, NavLocation.Location);
                SetCurrentState(EAIState::Exploring);
            }
            bRecovered = true;
        }
    }
    break;
```

### 5. Perception System Recovery

```cpp
case EAIErrorType::PerceptionError:
    // Try to reset perception system
    if (PerceptionComponent)
    {
        PerceptionComponent->ForgetAll();
        SetupPerceptionSystem();
        bRecovered = true;
    }
    break;
```

### 6. Task Execution Recovery

```cpp
case EAIErrorType::TaskExecutionFailed:
    // Generic recovery - restart behavior tree
    if (BehaviorTreeComponent && BehaviorTreeAsset)
    {
        BehaviorTreeComponent->RestartTree();
        bRecovered = true;
    }
    break;
```

## Debugging Tools

### 1. Visual Debugging

The system provides visual debugging in the game viewport:

```cpp
// In ReportAIError
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
```

### 2. Debug HUD

The Debug HUD provides a comprehensive view of AI status:

- **Toggle**: Press F3 to show/hide the debug HUD
- **AI Status**: Shows current state, target, and error information
- **Minimap**: Displays AI positions and targets
- **State Graph**: Visualizes time spent in each state

### 3. Console Logging

Detailed error information is logged to the console:

```cpp
// In ReportAIError
UE_LOG(LogTemp, Error, TEXT("AI Error: %s - %s"), 
       *UEnum::GetValueAsString(CurrentErrorState), *ErrorMessage);

// In TryRecoverFromError
if (bRecovered)
{
    UE_LOG(LogTemp, Warning, TEXT("AI recovered from error: %s"), 
           *UEnum::GetValueAsString(CurrentErrorState));
}
else
{
    UE_LOG(LogTemp, Error, TEXT("AI failed to recover from error: %s"), 
           *UEnum::GetValueAsString(CurrentErrorState));
}
```

### 4. Test Level

A dedicated test level for error handling:

- **Navigation Error Zone**: Tests getting stuck in dead-ends
- **Task Execution Error Zone**: Tests doors without matching keys
- **Path Following Error Zone**: Tests areas with no valid paths
- **Perception Error Zone**: Tests perception system failures

## Extending the System

### Adding New Error Types

1. Add a new enum value to `EAIErrorType`:

```cpp
enum class EAIErrorType : uint8
{
    // Existing error types...
    NewErrorType UMETA(DisplayName = "New Error Type"),
};
```

2. Add detection for the new error type:

```cpp
// Example detection in a relevant method
if (SomeErrorCondition)
{
    ReportAIError(EAIErrorType::NewErrorType, TEXT("Description of the new error"));
}
```

3. Add recovery strategy in `TryRecoverFromError`:

```cpp
case EAIErrorType::NewErrorType:
    // Implement recovery strategy
    // ...
    bRecovered = true; // if recovery was successful
    break;
```

### Adding New Recovery Strategies

1. Implement a new recovery method:

```cpp
bool AMazeBlazeAIController::TryAdvancedRecovery()
{
    // Implement advanced recovery logic
    // ...
    return bRecoverySuccessful;
}
```

2. Call the new method from `TryRecoverFromError`:

```cpp
case EAIErrorType::SomeErrorType:
    // Try standard recovery first
    bRecovered = StandardRecoveryMethod();
    
    // If standard recovery fails, try advanced recovery
    if (!bRecovered)
    {
        bRecovered = TryAdvancedRecovery();
    }
    break;
```

### Customizing Visual Debugging

1. Create a custom debug draw method:

```cpp
void AMazeBlazeAIController::DrawCustomDebugInfo()
{
    // Implement custom debug visualization
    // ...
}
```

2. Call it from appropriate places:

```cpp
// In Tick or a dedicated debug update method
if (bShowDebugInfo)
{
    DrawCustomDebugInfo();
}
```

## Best Practices

### 1. Error Detection

- **Be Specific**: Use the most specific error type for each issue
- **Provide Context**: Include detailed error messages
- **Avoid Spam**: Don't report the same error repeatedly
- **Check Early**: Detect errors as early as possible

### 2. Recovery Strategies

- **Progressive Recovery**: Try simple solutions before complex ones
- **Avoid Loops**: Ensure recovery doesn't cause more errors
- **Timeout Recovery**: Don't attempt recovery too frequently
- **Reset State**: Clear error state after successful recovery

### 3. Debugging

- **Use Visual Indicators**: Make errors visually obvious
- **Log Details**: Include enough information to diagnose issues
- **Track Recovery**: Log both success and failure of recovery attempts
- **Monitor Performance**: Ensure error handling doesn't impact performance

### 4. Integration

- **Add to Root Node**: Place error detection services at the root of behavior trees
- **Check Critical Points**: Add error checks at critical decision points
- **Graceful Degradation**: Allow AI to function with reduced capabilities when errors occur
- **Centralize Handling**: Use the AI controller as the central error handler

### 5. Testing

- **Test Each Error Type**: Verify detection and recovery for each error type
- **Stress Test**: Create scenarios that trigger multiple errors
- **Edge Cases**: Test recovery from rare or extreme conditions
- **Performance Testing**: Ensure error handling doesn't cause performance issues

---

This documentation provides a comprehensive overview of the MazeBlaze AI Error Handling System. By following these guidelines, you can create robust, self-healing AI that can recover from common errors and provide clear debugging information when issues occur.