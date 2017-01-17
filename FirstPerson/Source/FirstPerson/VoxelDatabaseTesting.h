#pragma once
#include "EngineMinimal.H"
#include "module/perlin.h"
#include "VoxelValueSources.h"
#include "VoxelMate.h"
#include "VoxelDatabaseTesting.generated.h"

UCLASS(Category = VoxelMateTesting, BlueprintType, Blueprintable)
class UVoxelPerlinNoiseSource : public UGridSource
{
	GENERATED_BODY()

public:
	UVoxelPerlinNoiseSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
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

	FORCEINLINE_DEBUGGABLE void GetValue_Implementation(float x, float y, float z, FVoxel& OutValue) const override
	{
		OutValue.Value = (float)PerlinNoise.GetValue((double)x, (double)y, (double)z);
	}

	UPROPERTY(Replicated, BlueprintReadOnly)
		float Frequency;
	UPROPERTY(Replicated, BlueprintReadOnly)
		float Lacunarity;
	UPROPERTY(Replicated, BlueprintReadOnly)
		float Persistence;
	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 OctaveCount;

private:
	noise::module::Perlin PerlinNoise;
};

UCLASS(Category = VoxelMateTesting, BlueprintType, Blueprintable)
class AVoxelMateTesting : public AActor
{
    GENERATED_BODY()

public:
	AVoxelMateTesting(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
	}
};
