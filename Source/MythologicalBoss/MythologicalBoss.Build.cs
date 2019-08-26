// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MythologicalBoss : ModuleRules
{
	public MythologicalBoss(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bFasterWithoutUnity = true;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "RHI", "MenuSystem", "ApexDestruction", "MediaAssets" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "MenuSystem", "Debug" });
    }
}
