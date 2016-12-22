#pragma once
#include "EngineMinimal.h"
#include "UnrealNetwork.h"
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

	UFUNCTION(Category = VoxelGridProxy, BlueprintNativeEvent, BlueprintCallable)
		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord) const;
	UFUNCTION(Category = VoxelGridProxy, BlueprintNativeEvent, BlueprintCallable)
		void FillValuesAndActiveState(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill);
	UFUNCTION(Category = VoxelGridProxy, BlueprintNativeEvent, BlueprintCallable)
		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface);

	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord)
	{
		return nullptr;
	}

	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill)
	{
	}

	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface)
	{
	}

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
	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;
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

	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;

	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
		bool IsFloatSavedAsHalf;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt32 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;
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

	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;

	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
		bool IsFloatSavedAsHalf;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyIntVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
	virtual AVoxelProxy* GetVoxel_Implementation(const FIntVector& IndexCoord) override;
	virtual void FillValuesAndActiveState_Implementation(const AVoxelProxy* VoxelProxy, const FIntVector& FillDimensions, bool VoxelizeActiveTilesAfterFill) override;
	virtual void SetValuesOnly_Implementation(EVoxelIterator VoxelIter, const TScriptInterface<IVoxelValueSourceInterface>& ValueSourceInterface) override;
};
