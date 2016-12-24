// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelValueSources.h"
#include "GridCoordinateTransforms.h"
#include "VoxelGridProxy.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

//TODO investigate UBlueprintFunctionLibrary
class FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFUNCTION(Category = VoxelMate, BlueprintCallable)
		bool AddVolume(UValueSource* ValueSource, const FText& GridDisplayText, FGuid& OutGridId);

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("VoxelMateModule");
    }
};
