// AIErrorHandlingTestCommands.h
// Console commands for running AI error handling tests during gameplay

#pragma once

#include "CoreMinimal.h"

/**
 * Static class for console commands to run AI error handling tests
 */
class MAZEBLAZE_API FAIErrorHandlingTestCommands
{
public:
    /** Run all AI error handling tests */
    static void RunAllTests();
    
    /** Run test for missing assets */
    static void RunAssetMissingTest();
    
    /** Run test for blackboard initialization failure */
    static void RunBlackboardInitFailedTest();
    
    /** Run test for behavior tree start failure */
    static void RunBehaviorTreeStartFailedTest();
    
    /** Run test for navigation system errors */
    static void RunNavigationMissingTest();
    
    /** Run test for perception system errors */
    static void RunPerceptionErrorTest();
    
    /** Run test for task execution failures */
    static void RunTaskExecutionFailedTest();

private:
    /** Run a specific test by name */
    static void RunSpecificTest(const FString& TestName);
    
    /** Get the current game world */
    static class UWorld* GetGameWorld();
};