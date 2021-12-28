// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MazeGame : ModuleRules
{
	public MazeGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
    new string[] {
                "MazeGame",
                "MazeGame/AI",
                "MazeGame/AI/BTServices",
                "MazeGame/AI/BTTasks",

    }
);

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks" });
	}
}
