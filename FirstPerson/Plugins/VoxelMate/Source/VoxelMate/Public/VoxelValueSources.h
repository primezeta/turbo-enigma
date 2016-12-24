#pragma once
#include "VoxelDatabaseTypes.h"
#include "VoxelValueSources.generated.h"

UCLASS(NotBlueprintable, NotPlaceable)
class VOXELMATE_API UValueSource : public UObject
{
	GENERATED_BODY()

public:
	UValueSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		IsFloatSavedAsHalf = false;
		VoxelType = EVoxelType::Bool;
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Ar << IsFloatSavedAsHalf;

		uint8 VoxelTypeValue = (uint8)VoxelType;
		Ar << VoxelTypeValue;
		uint8 CoordTransformTypeValue = (uint8)CoordTransformType;
		Ar << CoordTransformTypeValue;

		if (Ar.IsLoading())
		{
			VoxelType = (EVoxelType)VoxelTypeValue;
			CoordTransformType = (ECoordinateTransformType)CoordTransformTypeValue;
		}

		Ar << VolumeSize;
		Ar << CoordTransformData;
	}

	template<typename CoordTransform>
	void ReadCoordTransform(CoordTransform& OutCoordTransform)
	{
		FStreamReader<uint8> Reader(CoordTransformData);
		OutCoordTransform.Serialize(Reader);
	}

	template<typename CoordTransform>
	void WriteCoordTransform(const CoordTransform& InCoordTransform)
	{
		FStreamWriter<uint8> Writer(CoordTransformData);
		InCoordTransform.Serialize(Writer);
	}

	UPROPERTY()
		bool IsFloatSavedAsHalf;
	UPROPERTY(BlueprintReadWrite)
		EVoxelType VoxelType;
	UPROPERTY()
		ECoordinateTransformType CoordTransformType;
	UPROPERTY()
		FIntVector VolumeSize;
	UPROPERTY()
		TArray<uint8> CoordTransformData;
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelBoolSource : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelBoolSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::Bool;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelBool& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelBool& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelUInt8Source : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelUInt8Source(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::UInt8;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelUInt8& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelUInt8& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelFloatSource : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelFloatSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::Float;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelFloat& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelFloat& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelInt32Source : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelInt32Source(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::Int32;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelInt32& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelInt32& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelVectorSource : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelVectorSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::Vector;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelVector& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelVector& OutValue) const
	{
		check(false);
	}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API UVoxelIntVectorSource : public UValueSource
{
	GENERATED_BODY()

public:
	UVoxelIntVectorSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		VoxelType = EVoxelType::IntVector;
	}

	UFUNCTION(Category = VoxelValueSource, BlueprintNativeEvent, BlueprintCallable)
		void GetValue(float x, float y, float z, FVoxelIntVector& OutValue) const;
	virtual void GetValue_Implementation(float x, float y, float z, FVoxelIntVector& OutValue) const
	{
		check(false);
	}
};
