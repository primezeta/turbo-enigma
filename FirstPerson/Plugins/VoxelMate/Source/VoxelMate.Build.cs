// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoxelMate : ModuleRules
{
	public VoxelMate(TargetInfo Target)
	{
        bUseRTTI = true;
        bEnableExceptions = true;

        PublicIncludePaths.AddRange(
			new string[] {
				"VoxelMate/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"VoxelMate/Private",
				// ... add other private include paths required here ...
			}
			);


        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Projects",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UEOpenVdb",
				// ... add other public dependencies that you statically link with here ...
			}
            );
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "LibNoise",
                //"FastNoise",
                //"FastNoiseSIMD",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
