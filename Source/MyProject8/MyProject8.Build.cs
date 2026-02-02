// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject8 : ModuleRules
{
	public MyProject8(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
