// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "VoxelMatePCH.h"
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#include "EngineGridTypes.h"
#include "VoxelDatabase.h"
#include "VoxelDatabaseStatics.h"

#define LOCTEXT_NAMESPACE "FVoxelMateModule"

TArray<TSharedRef<FVoxelDatabase>> VoxelDatabases;

static FVoxelDatabase& FindOrAddDatabase(const FString& DatabaseName)
{
    //Search for the database by name as specified by the proxy and create a new database if it was not found
    TFunctionRef<bool(TSharedRef<FVoxelDatabase>&)> FindDbByName = [&](TSharedRef<FVoxelDatabase>& DbRef)
    {
        return DbRef->DatabaseName == DatabaseName;
    };

    check(!DatabaseName.IsEmpty());
    TSharedRef<FVoxelDatabase> DbRef;
    TSharedRef<FVoxelDatabase>* DbRefPtr = VoxelDatabases.FindByPredicate(FindDbByName);
    if (!DbRefPtr)
    {
        const bool EnableGridInstancing = true; //TODO where to change instancing option?
        const int32 Index = VoxelDatabases.Add(TSharedRef<FVoxelDatabase>(new FVoxelDatabase(DatabaseName, EnableGridInstancing)));
        DbRef = VoxelDatabases[Index];
    }
    else
    {
        DbRef = *DbRefPtr;
    }

    return DbRef.Get();
}

void FVoxelMateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    FVoxelDatabase::InitializeTypes();

//#if WITH_EDITOR
//    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
//    PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("TypedProperty"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FTypedPropertyCustomization::MakeInstance));
//#endif
}

void FVoxelMateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

UVoxelDatabaseProxy* FVoxelMateModule::OpenDatabaseProxy(UObject* ProxyOwner, const FString& DatabaseName, const FString& ProxyName)
{
    //Create the proxy which only knows the database by name
    UVoxelDatabaseProxy* Proxy = NewObject<UVoxelDatabaseProxy>(ProxyOwner);
    check(Proxy);
    Proxy->ProxyName = ProxyName;
    Proxy->DatabaseName = DatabaseName;
    FindOrAddDatabase(Proxy->DatabaseName);
    return Proxy;
}

void FVoxelMateModule::OpenDatabaseProxy(UVoxelDatabaseProxy* Proxy)
{
    if (Proxy) //TODO handle when proxy database name is bad
    {
        FindOrAddDatabase(Proxy->DatabaseName);
    }
}

void FVoxelMateModule::SerializeDatabase(UVoxelDatabaseProxy* Proxy, FArchive& Ar)
{
    if (Proxy && !Proxy->DatabaseName.IsEmpty())
    {
        FVoxelDatabase& Database = FindOrAddDatabase(Proxy->DatabaseName);
        Ar << Database;
    }
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

#undef LOCTEXT_NAMESPACE	
IMPLEMENT_MODULE(FVoxelMateModule, VoxelMate)
