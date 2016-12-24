// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelValueSources.h"
#include "GridCoordinateTransforms.h"
#include "VoxelGridProxy.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

//TODO investigate UBlueprintFunctionLibrary
class VOXELMATE_API FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFUNCTION(Category = VoxelMate)
		static void AddVolume(AValueSource* ValueSource, const FText& GridDisplayText);

	UFUNCTION(Category = VoxelMate)
		static void ChangeVoxelValue(const FGuid& GridId, const FIntVector& IndexCoord, const FVoxelBase& Voxel, bool IsActive);

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("VoxelMateModule");
    }
};
