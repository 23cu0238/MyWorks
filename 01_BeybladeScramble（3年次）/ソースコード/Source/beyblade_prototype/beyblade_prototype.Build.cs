// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class beyblade_prototype : ModuleRules
{
	public beyblade_prototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "ImageWrapper" , "EngineSettings" , "Niagara", "EnhancedInput", "UMG" });

        if (Target.bBuildEditor)//we only want this to be included for editor builds but not packaged builds
        {
            PublicDependencyModuleNames.AddRange(new string[]
                    {
                "UnrealEd",
                    });
        }

        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
