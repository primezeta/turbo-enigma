#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "ArchiveMetaValue.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

bool FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = false;
TArray<FString> FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames;

bool FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsRegisteredType(const FString& TypeName)
{
    return openvdb::Metadata::isRegisteredType(TCHAR_TO_UTF8(*TypeName));
}

FVoxelDatabaseTypeFactory<openvdb::Metadata>::ValueTypePtr FVoxelDatabaseTypeFactory<openvdb::Metadata>::CreateType(const FString& TypeName)
{
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::ValueTypePtr MetaValuePtr = nullptr;
    try
    {
        MetaValuePtr = openvdb::Metadata::createMetadata(TCHAR_TO_UTF8(*TypeName));
    }
    catch (const openvdb::LookupError& e)
    {
        (void)e; //TODO log error (metadata type not registered)
    }
    return MetaValuePtr;
}

void FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisterSupportedTypes()
{
    if (FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized)
    {
        return;
    }
    openvdb::Metadata::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames.Empty();
	FMetaValueFactory::Register<FMetadataBool>();
	FMetaValueFactory::Register<FMetadataFloat>();
	FMetaValueFactory::Register<FMetadataInt32>();
	FMetaValueFactory::Register<FMetadataUInt8>();
	FMetaValueFactory::Register<FMetadataVector>();
	FMetaValueFactory::Register<FMetadataColor>();
	FMetaValueFactory::Register<FMetadataLinearColor>();
	FMetaValueFactory::Register<FMetadataIntVector>();
	FMetaValueFactory::Register<FMetadataRotator>();
	FMetaValueFactory::Register<FMetadataQuat>();
	FMetaValueFactory::Register<FMetadataBox>();
	FMetaValueFactory::Register<FMetadataDateTime>();
	FMetaValueFactory::Register<FMetadataString>();
	FMetaValueFactory::Register<FMetadataText>();
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = true;
}

void FVoxelDatabaseTypeFactory<openvdb::Metadata>::UnregisterSupportedTypes()
{
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = false;
}
