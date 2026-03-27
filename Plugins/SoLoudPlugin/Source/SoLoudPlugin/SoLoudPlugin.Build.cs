// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SoLoudPlugin : ModuleRules
{
	public SoLoudPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		string SoLoudPath = Path.Combine(ModuleDirectory, "../ThirdParty/SoLoud");
		string Sdl2Path = Path.Combine(ModuleDirectory, "../ThirdParty/SDL2-2.30.0");

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(Sdl2Path, "include"),
			}
		);

		PrivateIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(SoLoudPath, "include"),
				Path.Combine(SoLoudPath, "src/core"),
				Path.Combine(SoLoudPath, "src/audiosource/wav"),
				Path.Combine(SoLoudPath, "src/backend/sdl2_static"),
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string SdlLibPath = Path.Combine(Sdl2Path, "lib", "x64", "SDL2.lib");
			string SdlDllPath = Path.Combine(Sdl2Path, "lib", "x64", "SDL2.dll");

			PublicAdditionalLibraries.Add(SdlLibPath);
			PublicDelayLoadDLLs.Add("SDL2.dll");
			RuntimeDependencies.Add("$(BinaryOutputDir)/SDL2.dll", SdlDllPath);
		}

		// Disabilitiamo i warning che bloccherebbero la build
		CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
		CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Off;

		// Mantengo la tua define esistente
		PublicDefinitions.Add("SOLOUD_DLL_EXPORT=1");

		// IMPORTANTE:
		// tolgo WITH_SDL2_STATIC=1 perché nel tuo setup stai usando SDL2.dll,
		// quindi non è un linking statico reale.

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings"
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}