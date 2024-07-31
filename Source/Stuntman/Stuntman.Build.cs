// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Stuntman : ModuleRules
{
    public Stuntman(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ChaosVehicles", "PhysicsCore", "UMG", "Json", "JsonUtilities", "AIModule" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
