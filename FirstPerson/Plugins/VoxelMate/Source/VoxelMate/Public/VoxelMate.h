// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelDatabase.h"

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
