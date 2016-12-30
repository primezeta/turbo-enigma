// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class VoxelMate : ModuleRules
{
	public VoxelMate(TargetInfo Target)
	{
        bUseRTTI = true;
        bEnableExceptions = true;

        PublicIncludePaths.AddRange(new string[]
            {
                "VoxelMate/Public",
            });

        PrivateIncludePaths.AddRange(new string[]
            {
                "VoxelMate/Private",
            });

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "Projects",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "UEOpenVdb",
                "RenderCore", //For GVectorPackingConstants (FPackedNormal)
            }
            );
	}
}
