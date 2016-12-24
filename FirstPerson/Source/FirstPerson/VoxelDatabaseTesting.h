#pragma once
#include "EngineMinimal.H"
#include "module/perlin.h"
#include "VoxelValueSources.h"
#include "VoxelMate.h"
#include "VoxelDatabaseTesting.generated.h"

UCLASS(Category = VoxelMateTesting, BlueprintType, Blueprintable)
class AVoxelPerlinNoiseSource : public AVoxelFloatSource
{
	GENERATED_BODY()

public:
	AVoxelPerlinNoiseSource(const FObjectInitializer& Initializer)
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

	VOXELMATEINLINE void GetValue_Implementation(float x, float y, float z, FVoxelFloat& OutValue) const override
	{
		OutValue.Value = (float)PerlinNoise.GetValue((double)x, (double)y, (double)z);
	}

	UPROPERTY(BlueprintReadWrite)
		float Frequency;
	UPROPERTY(BlueprintReadWrite)
		float Lacunarity;
	UPROPERTY(BlueprintReadWrite)
		float Persistence;
	UPROPERTY(BlueprintReadWrite)
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
		SetReplicates(true);
		PerlinNoiseSource = Initializer.CreateDefaultSubobject<AVoxelPerlinNoiseSource>(this, FName(NAME_Actor));
		PerlinNoiseSource->OnGridIdUpdated.AddDynamic(this, &AVoxelMateTesting::GridIsUpdated);
	}

	UPROPERTY(BlueprintReadOnly)
		AVoxelPerlinNoiseSource* PerlinNoiseSource;

	UPROPERTY()
		FGuid UpdatedGridId;

	UFUNCTION()
		void GridIsUpdated(const FGuid& GridId)
		{
			UpdatedGridId = GridId;
		}

	UFUNCTION(Category = VoxelMateTesting, BlueprintCallable)
		void AddPerlinNoiseVolume()
		{
			if (PerlinNoiseSource)
			{
				FVoxelMateModule::AddVolume(PerlinNoiseSource, FText::FromString(TEXT("PerlinNoise")));
			}
		}

	UFUNCTION(Category = VoxelMateTesting, BlueprintCallable)
		void ChangeVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelFloat& VoxelValue, bool IsActive)
		{
			FVoxelMateModule::ChangeVoxelValue(GridId, IndexCoord, VoxelValue, IsActive);
		}
};
