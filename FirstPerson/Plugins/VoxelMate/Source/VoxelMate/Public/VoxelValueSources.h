#pragma once
#include "VoxelValueSourceInterface.h"
#include "VoxelValueSources.generated.h"

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelBoolSource : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, bool& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, bool& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelUInt8Source : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, uint8& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, uint8& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelFloatSource : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, float& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, float& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelInt32Source : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, int32& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, int32& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelVectorSource : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVector& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVector& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable)
class VOXELMATE_API UVoxelIntVectorSource : public UObject, public IVoxelValueSourceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FIntVector& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FIntVector& OutValue) const
	{
		check(false);
	}
};
