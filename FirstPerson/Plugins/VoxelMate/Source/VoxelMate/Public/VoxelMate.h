// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelDatabaseCommon.h"
#include "VoxelValueSources.h"
#include "GridCoordinateTransforms.h"
#include "VoxelGridProxy.h"
#include "VoxelMate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

class FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("VoxelMateModule");
    }
};

UCLASS()
class VOXELMATE_API UVoxelMateLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void InitVoxelDatabase(AActor* ReplicationOwner);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void AddVolume(AValueSource* ValueSource, const FText& GridDisplayText);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeBoolVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBool& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeUInt8VoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelUInt8& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeInt32VoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelInt32& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeFloatVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelFloat& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeVectorVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelVector& Voxel, bool IsActive);

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		static void ChangeIntVectorVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelIntVector& Voxel, bool IsActive);
};
