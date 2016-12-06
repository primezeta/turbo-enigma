#pragma once
#include "EngineMinimal.H"
#include "module/perlin.h"
#include "VoxelValueSource.h"
#include "VoxelDatabaseTesting.generated.h"

UCLASS(Category = VoxelMateTesting, BlueprintType)
class UVoxelPerlinNoiseSource : public UObject, public IVoxelFloatSourceInterface
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
        float Frequency;
    UPROPERTY(BlueprintReadWrite)
        float Lacunarity;
    UPROPERTY(BlueprintReadWrite)
        float Persistence;
    UPROPERTY(BlueprintReadWrite)
        int32 OctaveCount;

    noise::module::Perlin PerlinNoise;

    UVoxelPerlinNoiseSource()
    {
        Frequency = 4.0f;
        Lacunarity = 0.49f;
        Persistence = 2.01f;
        OctaveCount = 9;
        InitNoise();
    }

    UFUNCTION(Category = VoxelMateTesting, BlueprintCallable)
        void InitNoise()
        {
            PerlinNoise.SetFrequency((double)Frequency);
            PerlinNoise.SetLacunarity((double)Lacunarity);
            PerlinNoise.SetPersistence((double)Persistence);
            PerlinNoise.SetOctaveCount(OctaveCount);
        }

    void GetValue_Implementation(float x, float y, float z, float& OutValue)
    {
        OutValue = (float)PerlinNoise.GetValue((double)x, (double)y, (double)z);
    }
};
