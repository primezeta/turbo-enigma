// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "VoxelDatabaseProxy.h"
#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelMate, Log, All)

class FVoxelMateModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

    static UVoxelDatabaseProxy* OpenDatabaseProxy(UObject* ProxyOwner, const FString& DatabaseName, const FString& ProxyName);
    static void OpenDatabaseProxy(UVoxelDatabaseProxy* Proxy);
    static void SerializeDatabase(UVoxelDatabaseProxy* Proxy, FArchive& Ar);
};
