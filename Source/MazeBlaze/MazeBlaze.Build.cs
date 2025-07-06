// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MazeBlaze : ModuleRules
{
	public MazeBlaze(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"AIModule", 
			"NavigationSystem", 
			"GameplayTasks", 
			"UMG" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Slate UI is required for UMG
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Add test framework dependencies
		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[] {
				"UnrealEd",
				"FunctionalTesting",
				"AutomationController"
			});
		}

		// Add automation testing support
		PublicDependencyModuleNames.Add("AutomationTest");
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
