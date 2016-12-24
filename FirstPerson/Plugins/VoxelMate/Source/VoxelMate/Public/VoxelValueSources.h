#pragma once
#include "Delegate.h"
#include "VoxelDatabaseTypes.h"
#include "VoxelValueSources.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridIdChanged, const FGuid&, GridId);

UCLASS(Abstract, NotBlueprintable, NotPlaceable)
class VOXELMATE_API AValueSource : public AActor
{
	GENERATED_BODY()

public:
	AValueSource(const FObjectInitializer& Initializer)
		: Super(Initializer)
	{
		SetReplicates(true);
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
		
	UPROPERTY(ReplicatedUsing=OnRep_GridId, BlueprintReadOnly)
		FGuid GridId;
	UPROPERTY()
		bool IsFloatSavedAsHalf;
	UPROPERTY(BlueprintReadOnly)
		EVoxelType VoxelType;
	UPROPERTY(BlueprintReadWrite)
		ECoordinateTransformType CoordTransformType;
	UPROPERTY(BlueprintReadWrite)
		FIntVector VolumeSize;
	UPROPERTY()
		TArray<uint8> CoordTransformData;

	UPROPERTY(BlueprintAssignable)
		FGridIdChanged OnGridIdUpdated;

	UFUNCTION()
		void OnRep_GridId()
		{
			OnGridIdUpdated.Broadcast(GridId);
		}
};

UCLASS(Abstract, Blueprintable, NotPlaceable)
class VOXELMATE_API AVoxelBoolSource : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelBoolSource(const FObjectInitializer& Initializer)
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
class VOXELMATE_API AVoxelUInt8Source : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelUInt8Source(const FObjectInitializer& Initializer)
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
class VOXELMATE_API AVoxelFloatSource : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelFloatSource(const FObjectInitializer& Initializer)
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
class VOXELMATE_API AVoxelInt32Source : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelInt32Source(const FObjectInitializer& Initializer)
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
class VOXELMATE_API AVoxelVectorSource : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelVectorSource(const FObjectInitializer& Initializer)
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
class VOXELMATE_API AVoxelIntVectorSource : public AValueSource
{
	GENERATED_BODY()

public:
	AVoxelIntVectorSource(const FObjectInitializer& Initializer)
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
