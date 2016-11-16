// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "ModuleManager.h"
#include "VoxelDatabaseProxy.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

//TODO investigate UBlueprintFunctionLibrary
class VOXELMATE_API FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
