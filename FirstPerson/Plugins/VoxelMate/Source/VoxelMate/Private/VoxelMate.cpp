// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "VoxelMatePCH.h"
#include "Core.h"
#include "UnrealNetwork.h"
#include "ModuleManager.h"
#include "IPluginManager.h"
#include "VoxelDatabase.h"

#define LOCTEXT_NAMESPACE "FVoxelMateModule"

void FVoxelMateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	//if (!DatabaseAssetLibrary)
	//{
	//	DatabaseAssetLibrary = UObjectLibrary::CreateLibrary(AVoxelDatabase::StaticClass(), false, GIsEditor);
	//	DatabaseAssetLibrary->AddToRoot();
	//}

	//DatabaseAssetLibrary->LoadAssetDataFromPath(DatabaseAssetPath);

	//TArray<FAssetData> AssetData;
	//DatabaseAssetLibrary->GetAssetDataList(AssetData);

	//for (auto i = AssetData.CreateIterator(); i; ++i)
	//{
	//	FAssetData& AssetData = *i;
	//	const FString* FindTypeNameString = AssetData.TagsAndValues.Find(GET_MEMBER_NAME_CHECKED(AVoxelDatabase, DatabaseDirectory));
	//	if (FindTypeNameString)
	//	{
	//		DatabaseDataStreamManager.RequestAsyncLoad(AssetData.ToStringReference(), FVoxelMateModule::LoadVoxelDatabase);
	//	}
	//}

	AVoxelDatabase::Startup();
}

void FVoxelMateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	AVoxelDatabase::Shutdown();
}

const FString VoxelDatabaseStatics::GridStatics::HalfFloatTypenameSuffix = TEXT("_HalfFloat");
const FString VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText = TEXT("GridDisplayText");
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

void AVoxelProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxy, IndexCoord);
	DOREPLIFETIME(AVoxelProxy, IsVoxelActive);
}

void AVoxelProxyBool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyBool, VoxelValue);
}

void AVoxelProxyFloat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyFloat, VoxelValue);
}

void AVoxelProxyInt32::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyInt32, VoxelValue);
}

void AVoxelProxyUInt8::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyUInt8, VoxelValue);
}

void AVoxelProxyVector::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyVector, VoxelValue);
}

void AVoxelProxyIntVector::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelProxyIntVector, VoxelValue);
}

#undef LOCTEXT_NAMESPACE	
IMPLEMENT_MODULE(FVoxelMateModule, VoxelMate)
