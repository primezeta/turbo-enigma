// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "VoxelMatePrivatePCH.h"
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"
#include "VoxelDatabase.h"

#define LOCTEXT_NAMESPACE "FVoxelMateModule"

void FVoxelMateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FVoxelMateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

const FString VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayName = TEXT("GridDisplayName");
const FString VoxelDatabaseStatics::GridStatics::MetaNameGridClass       = UTF8_TO_TCHAR(openvdb::GridBase::META_GRID_CLASS);
const FString VoxelDatabaseStatics::GridStatics::MetaNameGridCreator     = UTF8_TO_TCHAR(openvdb::GridBase::META_GRID_CREATOR);
const FString VoxelDatabaseStatics::GridStatics::MetaNameGridName        = UTF8_TO_TCHAR(openvdb::GridBase::META_GRID_NAME);
const FString VoxelDatabaseStatics::GridStatics::MetaNameSaveHalfFloat   = UTF8_TO_TCHAR(openvdb::GridBase::META_SAVE_HALF_FLOAT);
const FString VoxelDatabaseStatics::GridStatics::MetaNameIsLocalSpace    = UTF8_TO_TCHAR(openvdb::GridBase::META_IS_LOCAL_SPACE);
const FString VoxelDatabaseStatics::GridStatics::MetaNameVectorType      = UTF8_TO_TCHAR(openvdb::GridBase::META_VECTOR_TYPE);
const FString VoxelDatabaseStatics::GridStatics::MetaNameFileBBoxMin     = UTF8_TO_TCHAR(openvdb::GridBase::META_FILE_BBOX_MIN);
const FString VoxelDatabaseStatics::GridStatics::MetaNameFileBBoxMax     = UTF8_TO_TCHAR(openvdb::GridBase::META_FILE_BBOX_MAX);
const FString VoxelDatabaseStatics::GridStatics::MetaNameFileCompression = UTF8_TO_TCHAR(openvdb::GridBase::META_FILE_COMPRESSION);
const FString VoxelDatabaseStatics::GridStatics::MetaNameFileMemBytes    = UTF8_TO_TCHAR(openvdb::GridBase::META_FILE_MEM_BYTES);

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoxelMateModule, VoxelMate)
