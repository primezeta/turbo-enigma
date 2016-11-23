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
        CellularReturnType(ENoiseCellularReturnType::Distance)
    {
    }

    UPROPERTY(BlueprintReadWrite)
        ENoiseType NoiseType;
    UPROPERTY(BlueprintReadWrite)
        ENoiseFractalMode FractalMode;
    UPROPERTY(BlueprintReadWrite)
        ENoiseCellularDistanceMode CellularDistanceMode;
    UPROPERTY(BlueprintReadWrite)
        ENoiseCellularReturnType CellularReturnType;
};
