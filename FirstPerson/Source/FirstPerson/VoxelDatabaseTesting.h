#pragma once
#include "EngineMinimal.H"
#include "module/perlin.h"
#include "VoxelValueSources.h"
#include "VoxelDatabaseTesting.generated.h"

UCLASS(Category = VoxelMateTesting, BlueprintType, Blueprintable)
class UVoxelPerlinNoiseSource : public UVoxelFloatSource
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

	UVoxelPerlinNoiseSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
    {
        Frequency = 4.0f;
        Lacunarity = 0.49f;
        Persistence = 2.01f;
        OctaveCount = 9;
        InitNoise();
    }

    UFUNCTION(Category = VoxelMateTesting, BlueprintPure, BlueprintCallable)
        static UVoxelPerlinNoiseSource* CreatePerlinNoiseSource()
        {
            UVoxelPerlinNoiseSource* PerlinNoiseSource = NewObject<UVoxelPerlinNoiseSource>();
            PerlinNoiseSource->InitNoise();
            return PerlinNoiseSource;
        }

    UFUNCTION(Category = VoxelMateTesting, BlueprintCallable)
        void InitNoise()
        {
            PerlinNoise.SetFrequency((double)Frequency);
            PerlinNoise.SetLacunarity((double)Lacunarity);
            PerlinNoise.SetPersistence((double)Persistence);
            PerlinNoise.SetOctaveCount(OctaveCount);
        }

    FORCEINLINE void GetValue_Implementation(float x, float y, float z, FVoxelFloat& OutValue) const override
    {
        OutValue.Value = (float)PerlinNoise.GetValue((double)x, (double)y, (double)z);
    }
};
