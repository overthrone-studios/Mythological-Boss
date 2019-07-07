// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MythologicalBoss : ModuleRules
{
	public MythologicalBoss(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "RHI", "MenuSystem" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "MenuSystem" });
    }
}
