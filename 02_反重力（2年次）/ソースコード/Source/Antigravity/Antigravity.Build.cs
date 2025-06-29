// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Antigravity : ModuleRules
{
	public Antigravity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" , "NavigationSystem" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem", "Slate", "SlateCore" });
    }
}
