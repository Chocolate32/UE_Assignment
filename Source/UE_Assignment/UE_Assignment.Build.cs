// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_Assignment : ModuleRules
{
	public UE_Assignment(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
