# MazeBlaze AI Error Handling Test Framework

This test framework provides automated testing for the AI error handling system in the MazeBlaze project. It allows you to test how the AI controllers respond to various error conditions and verify that the error recovery mechanisms work correctly.

## Overview

The test framework includes:

1. **Automated Tests** - Tests that can be run through Unreal's automation system
2. **Manual Tests** - Tests that can be triggered during gameplay via console commands
3. **Test Results Reporting** - Detailed reports on test outcomes and recovery metrics

## Error Types Tested

The framework tests the following error types:

1. **AssetMissing** - Tests how the AI handles missing behavior tree or blackboard assets
2. **BlackboardInitFailed** - Tests recovery from blackboard initialization failures
3. **BehaviorTreeStartFailed** - Tests recovery from behavior tree start failures
4. **NavigationMissing** - Tests how the AI handles navigation system errors
5. **PerceptionError** - Tests recovery from perception system failures
6. **TaskExecutionFailed** - Tests recovery from behavior tree task execution failures

## How to Run Tests

### Using Automation Testing

1. Open the Unreal Editor
2. Go to Window > Test Automation
3. In the Automation window, find and check "MazeBlaze.AIErrorHandling"
4. Click "Start Tests" to run all AI error handling tests

### Using Console Commands

During gameplay, you can use the following console commands to run tests:

- `MazeBlaze.RunAllAIErrorTests` - Runs all AI error handling tests
- `MazeBlaze.RunAIErrorTest.AssetMissing` - Tests handling of missing assets
- `MazeBlaze.RunAIErrorTest.BlackboardInitFailed` - Tests blackboard initialization failure
- `MazeBlaze.RunAIErrorTest.BehaviorTreeStartFailed` - Tests behavior tree start failure
- `MazeBlaze.RunAIErrorTest.NavigationMissing` - Tests navigation system errors
- `MazeBlaze.RunAIErrorTest.PerceptionError` - Tests perception system errors
- `MazeBlaze.RunAIErrorTest.TaskExecutionFailed` - Tests task execution failures

### From C++ Code

You can also run tests programmatically from your C++ code:

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

## Test Results

The test framework generates detailed reports that include:

- Number of tests run, passed, and failed
- Number of recovery attempts and successes
- Average recovery time
- Detailed results for each individual test
- Error messages for failed tests

## Extending the Tests

To add new tests for additional error types:

1. Add a new test method to `FAIErrorHandlingTestFramework`
2. Update the `RunAllTests()` and `RunTest()` methods to include your new test
3. Add a new console command in `AIErrorHandlingTestCommands.cpp`

## Best Practices

- Run tests in a controlled environment with minimal distractions for the AI
- Use a test level with proper navigation mesh setup
- Place multiple AI controllers in the level for more thorough testing
- Check the logs for detailed information about test failures
- Run tests after making changes to the AI error handling system

## Troubleshooting

- If tests fail to run, ensure there are AI controllers in the level
- If recovery tests fail, check the recovery logic in `MazeBlazeAIController`
- For navigation tests, ensure your level has a proper navigation mesh
- If tests hang, check for infinite loops in the error recovery code