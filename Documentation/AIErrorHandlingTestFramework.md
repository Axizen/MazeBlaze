# AI Error Handling Test Framework

## Overview

The AI Error Handling Test Framework is a comprehensive testing solution designed to validate the robustness of the MazeBlaze AI error handling system. It provides automated tests for various error conditions that AI controllers might encounter during gameplay, ensuring that the error detection and recovery mechanisms work correctly.

## Components

The test framework consists of the following components:

### 1. Test Framework Core (AIErrorHandlingTests.h/cpp)

The core testing framework that defines test methods for each error type and provides functionality to run tests and collect results.

- `FAIErrorTestResults`: Structure to store and report test results
- `FAIErrorHandlingTestFramework`: Main class that implements test methods
- `RunAIErrorHandlingTests()`: Utility function to run tests from anywhere

### 2. Console Commands (AIErrorHandlingTestCommands.h/cpp)

Console commands that allow running tests during gameplay without modifying code.

- `MazeBlaze.RunAllAIErrorTests`: Run all error handling tests
- `MazeBlaze.RunAIErrorTest.[ErrorType]`: Run a specific error test

### 3. Test Level Helper (AIErrorHandlingTestLevel.h/cpp)

An actor class that helps set up test environments for AI error handling tests.

- Spawns test AI characters
- Creates test zones for different error types
- Provides methods to trigger errors and reset AI

## Error Types Tested

The framework tests the following error types:

### 1. AssetMissing

Tests how the AI handles missing behavior tree or blackboard assets.

**Detection Method**: Sets the behavior tree asset to null and attempts to restart the AI controller.

**Expected Recovery**: The AI should detect the missing asset and report an error. Full recovery is not expected since assets cannot be created at runtime.

### 2. BlackboardInitFailed

Tests recovery from blackboard initialization failures.

**Detection Method**: Simulates a blackboard initialization failure by directly setting the error state.

**Expected Recovery**: The AI should attempt to reinitialize the blackboard and continue operation.

### 3. BehaviorTreeStartFailed

Tests recovery from behavior tree start failures.

**Detection Method**: Simulates a behavior tree start failure by directly setting the error state.

**Expected Recovery**: The AI should attempt to restart the behavior tree and continue operation.

### 4. NavigationMissing

Tests how the AI handles navigation system errors.

**Detection Method**: Teleports the AI to a location without navigation and forces it to move.

**Expected Recovery**: The AI should detect the navigation error and attempt to find a valid navigation point.

### 5. PerceptionError

Tests recovery from perception system failures.

**Detection Method**: Simulates a perception system failure by directly setting the error state.

**Expected Recovery**: The AI should reset the perception system and continue operation.

### 6. TaskExecutionFailed

Tests recovery from behavior tree task execution failures.

**Detection Method**: Simulates a task execution failure by directly setting the error state.

**Expected Recovery**: The AI should restart the behavior tree and continue operation.

## How to Use

### Running Tests Through Automation System

1. Open the Unreal Editor
2. Go to Window > Test Automation
3. In the Automation window, find and check "MazeBlaze.AIErrorHandling"
4. Click "Start Tests" to run all AI error handling tests

### Running Tests Through Console Commands

During gameplay, you can use the following console commands:

```
MazeBlaze.RunAllAIErrorTests
MazeBlaze.RunAIErrorTest.AssetMissing
MazeBlaze.RunAIErrorTest.BlackboardInitFailed
MazeBlaze.RunAIErrorTest.BehaviorTreeStartFailed
MazeBlaze.RunAIErrorTest.NavigationMissing
MazeBlaze.RunAIErrorTest.PerceptionError
MazeBlaze.RunAIErrorTest.TaskExecutionFailed
```

### Running Tests Programmatically

```cpp
#include "Tests/AIErrorHandlingTests.h"

// Run all tests
UWorld* World = GetWorld();
int32 Result = RunAIErrorHandlingTests(World);

// Or use the test framework directly
FAIErrorHandlingTestFramework TestFramework(World);
bool bAllTestsPassed = TestFramework.RunAllTests();

// Run a specific test
bool bTestPassed = TestFramework.RunTest(TEXT("NavigationMissing"));

// Get detailed test results
const FAIErrorTestResults& Results = TestFramework.GetTestResults();
FString Report = Results.GenerateReport();
```

### Setting Up a Test Level

1. Create a new level
2. Add the `AAIErrorHandlingTestLevel` actor to the level
3. Configure the actor properties:
   - Set the AI character class to spawn
   - Set the behavior tree and blackboard to use
   - Enable auto-run tests if desired
4. Play the level to run tests

## Test Results

The test framework generates detailed reports that include:

- Number of tests run, passed, and failed
- Number of recovery attempts and successes
- Average recovery time
- Detailed results for each individual test
- Error messages for failed tests

Example report:

```
===== AI ERROR HANDLING TEST REPORT =====

Total Tests: 7
Successful Tests: 6 (85.7%)
Failed Tests: 1 (14.3%)

Recovery Attempts: 6
Successful Recoveries: 5 (83.3%)
Average Recovery Time: 0.42 seconds

Individual Test Results:
  AIControllerCheck: PASS
  AssetMissingTest: PASS
  BlackboardInitFailedTest: PASS
  BehaviorTreeStartFailedTest: PASS
  NavigationMissingTest: FAIL
  PerceptionErrorTest: PASS
  TaskExecutionFailedTest: PASS

Error Messages:
  - NavigationMissingTest: Failed to detect navigation error

========================================
```

## Extending the Framework

### Adding New Error Types

1. Add a new error type to the `EAIErrorType` enum in `MazeBlazeAIController.h`
2. Add a new test method to `FAIErrorHandlingTestFramework` in `AIErrorHandlingTests.cpp`
3. Update the `RunAllTests()` and `RunTest()` methods to include your new test
4. Add a new console command in `AIErrorHandlingTestCommands.cpp`

Example of adding a new test method:

```cpp
void FAIErrorHandlingTestFramework::TestNewErrorType(const TArray<AMazeBlazeAIController*>& AIControllers)
{
    UE_LOG(LogTemp, Display, TEXT("Running NewErrorType test..."));
    
    if (AIControllers.Num() == 0) return;
    
    AMazeBlazeAIController* TestController = AIControllers[0];
    if (!TestController) return;
    
    // Simulate the new error type
    StartRecoveryTimer();
    TestController->ReportAIError(AMazeBlazeAIController::EAIErrorType::NewErrorType, 
                                TEXT("Simulated new error type"));
    
    // Wait for recovery attempt
    for (int32 i = 0; i < 10; ++i)
    {
        FPlatformProcess::Sleep(0.1f);
    }
    
    // Check if recovery was attempted
    float RecoveryTime = GetRecoveryTime();
    bool bRecovered = !TestController->IsInErrorState();
    TestResults.RecordRecoveryAttempt(bRecovered, RecoveryTime);
    
    TestResults.AddTestResult(TEXT("NewErrorTypeTest"), true);
}
```

### Creating Custom Test Scenarios

You can create custom test scenarios by extending the `AAIErrorHandlingTestLevel` class:

```cpp
class AMyCustomTestLevel : public AAIErrorHandlingTestLevel
{
    GENERATED_BODY()

public:
    // Create a custom test scenario
    UFUNCTION(BlueprintCallable, Category = "AI Testing")
    void RunCustomTestScenario();
};
```

## Best Practices

### Test Environment

- Use a dedicated test level with proper navigation mesh
- Ensure the level has enough space for AI movement
- Include obstacles and navigation challenges
- Place multiple AI controllers for more thorough testing

### Test Execution

- Run tests after making changes to the AI error handling system
- Run tests in isolation to avoid interference
- Check logs for detailed information about test failures
- Use the visual debugging features to observe AI behavior

### Test Maintenance

- Keep tests up to date with changes to the error handling system
- Add new tests for new error types
- Refine existing tests based on real-world issues
- Document test scenarios and expected results

## Troubleshooting

### Common Issues

- **Tests fail to run**: Ensure there are AI controllers in the level
- **Recovery tests fail**: Check the recovery logic in `MazeBlazeAIController`
- **Navigation tests fail**: Ensure your level has a proper navigation mesh
- **Tests hang**: Check for infinite loops in the error recovery code

### Debugging Tips

- Enable visual debugging to see what's happening
- Check the logs for detailed error messages
- Use breakpoints in the test methods to step through the code
- Try running individual tests to isolate issues

## Conclusion

The AI Error Handling Test Framework provides a comprehensive solution for testing the robustness of the MazeBlaze AI error handling system. By automating tests for various error conditions, it helps ensure that AI controllers can detect and recover from errors during gameplay, resulting in a more reliable and enjoyable player experience.