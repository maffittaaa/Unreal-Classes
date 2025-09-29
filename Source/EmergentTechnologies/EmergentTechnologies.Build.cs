// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EmergentTechnologies : ModuleRules
{
	public EmergentTechnologies(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EmergentTechnologies",
			"EmergentTechnologies/Variant_Platforming",
			"EmergentTechnologies/Variant_Platforming/Animation",
			"EmergentTechnologies/Variant_Combat",
			"EmergentTechnologies/Variant_Combat/AI",
			"EmergentTechnologies/Variant_Combat/Animation",
			"EmergentTechnologies/Variant_Combat/Gameplay",
			"EmergentTechnologies/Variant_Combat/Interfaces",
			"EmergentTechnologies/Variant_Combat/UI",
			"EmergentTechnologies/Variant_SideScrolling",
			"EmergentTechnologies/Variant_SideScrolling/AI",
			"EmergentTechnologies/Variant_SideScrolling/Gameplay",
			"EmergentTechnologies/Variant_SideScrolling/Interfaces",
			"EmergentTechnologies/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
