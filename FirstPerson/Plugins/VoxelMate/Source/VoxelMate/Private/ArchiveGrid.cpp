#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>

bool FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = false;
TArray<FString> FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeNames;

bool FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsRegisteredType(const FString& TypeName)
{
    return openvdb::GridBase::isRegistered(TCHAR_TO_UTF8(*TypeName));
}

void FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisterSupportedTypes()
{
    if (FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized)
    {
        return;
    }
    openvdb::GridBase::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::RegisteredTypeNames.Empty();
    FGridFactory::Register<FVoxelBool>();
    FGridFactory::Register<FVoxelUInt8>();
    FGridFactory::Register<FVoxelInt32>();
    FGridFactory::Register<FVoxelFloat>();
    FGridFactory::Register<FVoxelVector>();
    FGridFactory::Register<FVoxelIntVector>();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = true;
}

void FVoxelDatabaseTypeFactory<openvdb::GridBase>::UnregisterSupportedTypes()
{
    openvdb::GridBase::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::GridBase>::IsInitialized = false;
}
