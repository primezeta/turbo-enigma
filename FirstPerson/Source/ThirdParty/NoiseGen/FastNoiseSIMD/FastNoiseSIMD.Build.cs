// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

enum SimdArch { NONE, SSE, SSE2, SSE41, AVX, AVX2, AVX2_FMA3 }

public class FastNoiseSIMD : ModuleRules
{
    private UnrealTargetPlatform Platform;
    private UnrealTargetConfiguration Configuration;
    private string NoiseLib;

    public FastNoiseSIMD(TargetInfo Target)
    {
        Platform = Target.Platform;
        Configuration = Target.Configuration;
        Type = ModuleType.External;
        OptimizeCode = CodeOptimization.InNonDebugBuilds;
        SimdArch Arch = SimdArch.AVX2;

        //TODO: Figure out how to configure FN_ALIGNED_SETS from FastNoiseSIMD
        bUseAVX = (Arch == SimdArch.AVX || Arch == SimdArch.AVX2 || Arch == SimdArch.AVX2_FMA3);
        if (Arch == SimdArch.SSE)
        {
            NoiseLib = "FastNoiseSIMD_sse.lib";
            Definitions.Add("FN_COMPILE_SSE2");
        }
        else if (Arch == SimdArch.SSE2)
        {
            NoiseLib = "FastNoiseSIMD_sse2.lib";
            Definitions.Add("FN_COMPILE_SSE2");
        }
        else if (Arch == SimdArch.SSE41)
        {
            NoiseLib = "FastNoiseSIMD_sse41.lib";
            Definitions.Add("FN_COMPILE_SSE41");
        }
        else if (Arch == SimdArch.AVX)
        {
            NoiseLib = "FastNoiseSIMD_avx.lib";
            Definitions.Add("FN_COMPILE_AVX2");
        }
        else if (Arch == SimdArch.AVX2)
        {
            NoiseLib = "FastNoiseSIMD_avx2.lib";
            Definitions.Add("FN_COMPILE_AVX2");
        }
        else if (Arch == SimdArch.AVX2_FMA3)
        {
            NoiseLib = "FastNoiseSIMD_avx2_fma3.lib";
            Definitions.Add("FN_COMPILE_AVX2");
            Definitions.Add("FN_USE_FMA3");
        }
        else
        {
            Console.WriteLine("Warning: FastNoiseSIMD falling back to no SIMD usage");
            Definitions.Add("FastNoiseSIMD.lib");
        }

        PublicIncludePaths.AddRange(PublicIncludes);
        PrivateIncludePaths.AddRange(PrivateIncludes);
        PublicSystemIncludePaths.AddRange(ThirdPartyIncludes);
        PublicLibraryPaths.AddRange(ThirdPartyLibPaths);
        PublicAdditionalLibraries.AddRange(ThirdPartyLibNames);
    }
    
    private string PlatformPath
    {
        get
        {
            string path = "Win32";
            if (Platform == UnrealTargetPlatform.Win64)
            {
                path = "x64";
            }
            return path;
        }
    }

    private string ConfigurationPath
    {
        get
        {
            string path = "Release";
            if(Configuration == UnrealTargetConfiguration.Debug ||
               Configuration == UnrealTargetConfiguration.Development)
            {
                path = "Debug";
            }
            return path;
        }
    }

    private string[] PublicIncludes
    {
        get
        {
            return new string[]
            {
                Path.Combine(ModuleDirectory),
            };
        }
    }

    private string[] PrivateIncludes
    {
        get
        {
            return new string[]
            {
            };
        }
    }

    private string ThirdPartyPath
    {
        get
        {
            return Path.Combine(ModuleDirectory, "..", "..");
        }
    }

    private string[] ThirdPartyIncludes
    {
        get
        {
            return new string[]
            {
            };
        }
    }

    private string[] ThirdPartyLibPaths
    {
        get
        {
            return new string[]
            {
                Path.Combine(ModuleDirectory, "..", "..", "Binaries", "VS2015", PlatformPath, ConfigurationPath),
            };
        }
    }

    private string[] ThirdPartyLibNames
    {
        get
        {
            return new string[]
            {
                NoiseLib,
            };
        }
    }    
}
