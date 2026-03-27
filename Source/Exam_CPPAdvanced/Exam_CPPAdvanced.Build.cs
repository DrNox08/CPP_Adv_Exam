// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Exam_CPPAdvanced : ModuleRules
{
	public Exam_CPPAdvanced(ReadOnlyTargetRules Target) : base(Target)
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
			"Exam_CPPAdvanced",
			"Exam_CPPAdvanced/Variant_Platforming",
			"Exam_CPPAdvanced/Variant_Platforming/Animation",
			"Exam_CPPAdvanced/Variant_Combat",
			"Exam_CPPAdvanced/Variant_Combat/AI",
			"Exam_CPPAdvanced/Variant_Combat/Animation",
			"Exam_CPPAdvanced/Variant_Combat/Gameplay",
			"Exam_CPPAdvanced/Variant_Combat/Interfaces",
			"Exam_CPPAdvanced/Variant_Combat/UI",
			"Exam_CPPAdvanced/Variant_SideScrolling",
			"Exam_CPPAdvanced/Variant_SideScrolling/AI",
			"Exam_CPPAdvanced/Variant_SideScrolling/Gameplay",
			"Exam_CPPAdvanced/Variant_SideScrolling/Interfaces",
			"Exam_CPPAdvanced/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
