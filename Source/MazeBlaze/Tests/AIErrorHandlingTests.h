// AIErrorHandlingTests.h
// Header file for the MazeBlaze AI Error Handling Test Framework

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

/**
 * Structure to store test results for AI error handling tests
 */
struct MAZEBLAZE_API FAIErrorTestResults
{
    int32 TotalTests;
    int32 SuccessfulTests;
    int32 FailedTests;
    int32 RecoveryAttempts;
    int32 SuccessfulRecoveries;
    float AverageRecoveryTime;
    TMap<FString, bool> TestResults;
    TArray<FString> ErrorMessages;

    FAIErrorTestResults();
    void AddTestResult(const FString& TestName, bool bSuccess, const FString& ErrorMessage = TEXT(""));
    void RecordRecoveryAttempt(bool bSuccess, float RecoveryTime);
    FString GenerateReport() const;
};

/**
 * Class to run AI error handling tests
 * This can be used both in automation testing and manually from gameplay code
 */
class MAZEBLAZE_API FAIErrorHandlingTestFramework
{
public:
    FAIErrorHandlingTestFramework(UWorld* InWorld);

    /**
     * Runs all error handling tests
     * @return True if all tests passed, false otherwise
     */
    bool RunAllTests();

    /**
     * Runs a specific test by name
     * @param TestName The name of the test to run
     * @return True if the test passed, false otherwise
     */
    bool RunTest(const FString& TestName);

    /**
     * Gets the test results
     * @return The test results structure
     */
    const FAIErrorTestResults& GetTestResults() const { return TestResults; }

    // Test methods
    void TestAssetMissing(const TArray<class AMazeBlazeAIController*>& AIControllers);
    void TestBlackboardInitFailed(const TArray<class AMazeBlazeAIController*>& AIControllers);
    void TestBehaviorTreeStartFailed(const TArray<class AMazeBlazeAIController*>& AIControllers);
    void TestNavigationMissing(const TArray<class AMazeBlazeAIController*>& AIControllers);
    void TestPerceptionError(const TArray<class AMazeBlazeAIController*>& AIControllers);
    void TestTaskExecutionFailed(const TArray<class AMazeBlazeAIController*>& AIControllers);

    // Helper methods
    void StartRecoveryTimer();
    float GetRecoveryTime() const;

private:
    UWorld* World;
    FAIErrorTestResults TestResults;
    float CurrentTestStartTime;
};

/**
 * Function to run AI error handling tests manually
 * @param World The world to run tests in
 * @return 0 if all tests passed, 1 if any test failed, -1 if tests couldn't run
 */
MAZEBLAZE_API int32 RunAIErrorHandlingTests(UWorld* World);