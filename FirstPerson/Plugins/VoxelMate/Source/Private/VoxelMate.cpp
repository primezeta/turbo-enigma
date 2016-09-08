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

const FGridDatabaseString GridBaseStatics::MetaNameGridClass       = openvdb::GridBase::META_GRID_CLASS;
const FGridDatabaseString GridBaseStatics::MetaNameGridCreator     = openvdb::GridBase::META_GRID_CREATOR;
const FGridDatabaseString GridBaseStatics::MetaNameGridName        = openvdb::GridBase::META_GRID_NAME;
const FGridDatabaseString GridBaseStatics::MetaNameSaveHalfFloat   = openvdb::GridBase::META_SAVE_HALF_FLOAT;
const FGridDatabaseString GridBaseStatics::MetaNameIsLocalSpace    = openvdb::GridBase::META_IS_LOCAL_SPACE;
const FGridDatabaseString GridBaseStatics::MetaNameVectorType      = openvdb::GridBase::META_VECTOR_TYPE;
const FGridDatabaseString GridBaseStatics::MetaNameFileBBoxMin     = openvdb::GridBase::META_FILE_BBOX_MIN;
const FGridDatabaseString GridBaseStatics::MetaNameFileBBoxMax     = openvdb::GridBase::META_FILE_BBOX_MAX;
const FGridDatabaseString GridBaseStatics::MetaNameFileCompression = openvdb::GridBase::META_FILE_COMPRESSION;
const FGridDatabaseString GridBaseStatics::MetaNameFileMemBytes    = openvdb::GridBase::META_FILE_MEM_BYTES;

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoxelMateModule, VoxelMate)