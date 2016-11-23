#pragma once
#include "VoxelDatabaseCommon.h"
#include "NoiseGeneratorConfiguration.generated.h"

USTRUCT(BlueprintType)
struct FNoiseGeneratorConfiguration
{
    GENERATED_BODY()

    FNoiseGeneratorConfiguration() :
        NoiseType(ENoiseType::Value),
        FractalMode(ENoiseFractalMode::None),
        CellularDistanceMode(ENoiseCellularDistanceMode::Euclidean),
        CellularReturnType(ENoiseCellularReturnType::Distance),
        RandomSeed(FMath::Rand()),
        OctaveCount(3),
        Frequency(0.01f),
        Lacunarity(2.0f),
        Gain(0.5f),
        AxisScale(1.0f)
    {
    }

    friend uint32 GetTypeHash(const FNoiseGeneratorConfiguration& Config)
    {
        return HashCombine(GetTypeHash(Config.NoiseType),
            HashCombine(GetTypeHash(Config.FractalMode),
                HashCombine(GetTypeHash(Config.CellularDistanceMode),
                    HashCombine(GetTypeHash(Config.CellularReturnType),
                        HashCombine(GetTypeHash(Config.RandomSeed),
                            HashCombine(GetTypeHash(Config.OctaveCount),
                                HashCombine(GetTypeHash(Config.Frequency),
                                    HashCombine(GetTypeHash(Config.Lacunarity),
                                        HashCombine(GetTypeHash(Config.Gain), GetTypeHash(Config.AxisScale))))))))));
    }

    friend bool operator==(const FNoiseGeneratorConfiguration& InLhs, const FNoiseGeneratorConfiguration& InRhs)
    {
        return InLhs.NoiseType == InRhs.NoiseType &&
            InLhs.FractalMode == InRhs.FractalMode &&
            InLhs.CellularDistanceMode == InRhs.CellularDistanceMode &&
            InLhs.CellularReturnType == InRhs.CellularReturnType &&
            InLhs.RandomSeed == InRhs.RandomSeed &&
            InLhs.OctaveCount == InRhs.OctaveCount &&
            InLhs.Frequency == InRhs.Frequency &&
            InLhs.Lacunarity == InRhs.Lacunarity &&
            InLhs.Gain == InRhs.Gain &&
            InLhs.AxisScale == InRhs.AxisScale;
    }

    UPROPERTY(BlueprintReadWrite)
        ENoiseType NoiseType;
    UPROPERTY(BlueprintReadWrite)
        ENoiseFractalMode FractalMode;
    UPROPERTY(BlueprintReadWrite)
        ENoiseCellularDistanceMode CellularDistanceMode;
    UPROPERTY(BlueprintReadWrite)
        ENoiseCellularReturnType CellularReturnType;
    UPROPERTY(BlueprintReadWrite)
        int32 RandomSeed;
    UPROPERTY(BlueprintReadWrite)
        int32 OctaveCount;
    UPROPERTY(BlueprintReadWrite)
        float Frequency;
    UPROPERTY(BlueprintReadWrite)
        float Lacunarity;
    UPROPERTY(BlueprintReadWrite)
        float Gain;
    UPROPERTY(BlueprintReadWrite)
        FVector AxisScale;
};
