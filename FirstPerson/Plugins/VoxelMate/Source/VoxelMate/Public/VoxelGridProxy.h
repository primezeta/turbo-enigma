#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
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
        FIntVector GridId;
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly, Replicated)
        FText GridDisplayText;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
        TArray<AVoxelProxy*> ActiveVoxels;
};

//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyBool : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const bool& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelBoolSource>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//};
//
//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxyFloat& GridProxy)
//	{
//		if (!GridProxy.IsDefaultSubobject())
//		{
//			Ar << GridProxy.IsFloatSavedAsHalf;
//		}
//		return Ar;
//	}
//
//	virtual void Serialize(FArchive& Ar) override
//	{
//		Super::Serialize(Ar);
//		Ar << *this;
//	}
//
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const float& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelFloatSource>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//
//	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
//		bool IsFloatSavedAsHalf;
//};
//
//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyInt32 : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int32& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelInt32Source>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//};
//
//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint8& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelUInt8Source>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//};
//
//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyVector : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxyVector& GridProxy)
//	{
//		if (!GridProxy.IsDefaultSubobject())
//		{
//			Ar << GridProxy.IsFloatSavedAsHalf;
//		}
//		return Ar;
//	}
//
//	virtual void Serialize(FArchive& Ar) override
//	{
//		Super::Serialize(Ar);
//		Ar << *this;
//	}
//
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FVector& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelVectorSource>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//
//	UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
//		bool IsFloatSavedAsHalf;
//};
//
//UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
//class VOXELMATE_API AVoxelGridProxyIntVector : public AVoxelGridProxy
//{
//    GENERATED_BODY()
//
//public:
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		AVoxelProxy* GetVoxel(const FIntVector& IndexCoord);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void FillValuesAndActiveState(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FIntVector& Value, bool AreValuesActive, bool VoxelizeActiveTilesAfterFill);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void SetValuesOnly(EVoxelIterator VoxelIter, const TScriptInterface<UVoxelIntVectorSource>& ValueSourceInterface);
//	UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
//		void ExtractSurface(EVoxelIterator VoxelIter);
//};
