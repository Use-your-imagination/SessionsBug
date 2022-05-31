// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class SessionsBug : ModuleRules
{
	public SessionsBug(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Slate",
			"SlateCore",
			"UMG",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"PhysicsCore",
			"HeadMountedDisplay"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModuleDirectory),
			Path.Combine(EngineDirectory, "Plugins", "Online", "OnlineSubsystem", "Source", "Public")
		});

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
	}
}
