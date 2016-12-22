#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
#include "NoiseGeneratorConfiguration.h"
#include "GridCoordinateTransforms.h"
#include "VoxelDatabaseVoxelProxy.h"
#include "VoxelValueSources.h"
#include "VoxelGridProxy.generated.h"

//IDEA: The base configuration of each grid is defined only by the value source used to generate it.
//The server maintains a "diff grid" which contains only changed values and is uploaded to clients
//and clients apply it after generating the base grid.
UCLASS(ClassGroup = VoxelMate, Abstract, NotBlueprintable, NotPlaceable)
class VOXELMATE_API AVoxelGridProxy : public AActor
{
    GENERATED_BODY()

public:
	friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy)
    {
        if (!GridProxy.IsDefaultSubobject())
        {
            Ar << GridProxy.GridId;
            Ar << GridProxy.GridDisplayText;
            Ar << GridProxy.ActiveVoxels;
        }
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
        Ar << *this;
    }

    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly, Replicated)
        FGuid GridId;
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadWrite)
        FText GridDisplayText;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
        TArray<AVoxelProxy*> ActiveVoxels;

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToAffine(const FAffineCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToUnitary(const FUnitaryCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToScale(const FScaleCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToUniformScale(const FUniformScaleCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToTranslation(const FTranslationCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToScaleTranslation(const FScaleTranslationCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToUniformScaleTranslation(const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void ResetCoordinateTransformToNonlinearFrustum(const FNonlinearFrustumCoordinateTransform& InCoordinateTransform);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformTranslation(const FVector& InTranslation);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformScale(const FVector& InScale);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformUniformScale(float Scale);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void PrependCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformTranslation(const FVector& InTranslation);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformScale(const FVector& InScale);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformUniformScale(float Scale);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void AppendCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyBool : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const bool& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelBoolSource>& ValueSourceInterface);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxyFloat& GridProxy)
	{
		if (!GridProxy.IsDefaultSubobject())
		{
			Ar << GridProxy.IsFloatSavedAsHalf;
		}
		return Ar;
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Super::Serialize(Ar);
		Ar << *this;
	}

	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const float& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelFloatSource>& ValueSourceInterface);

	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
		bool IsFloatSavedAsHalf;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt32 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int32& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelInt32Source>& ValueSourceInterface);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint8& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelUInt8Source>& ValueSourceInterface);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxyVector& GridProxy)
	{
		if (!GridProxy.IsDefaultSubobject())
		{
			Ar << GridProxy.IsFloatSavedAsHalf;
		}
		return Ar;
	}

	virtual void Serialize(FArchive& Ar) override
	{
		Super::Serialize(Ar);
		Ar << *this;
	}

	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FVector& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelVectorSource>& ValueSourceInterface);

	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
		bool IsFloatSavedAsHalf;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyIntVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FIntVector& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelIntVectorSource>& ValueSourceInterface);
};
