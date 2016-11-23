#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "VoxelDatabase.h"
#include "ArchiveTransformMap.h"

void UVoxelDatabase::InitializeTypes()
{
    //TODO: Note that the following BLOSC related stuff is in openvdb::initialize
//#ifdef OPENVDB_USE_BLOSC
//    blosc_init();
//    if (blosc_set_compressor("lz4") < 0) {
//        OPENVDB_LOG_WARN("Blosc LZ4 compressor is unavailable");
//    }
//    /// @todo blosc_set_nthreads(int nthreads);
//#endif
    FGridFactory::RegisterSupportedTypes();
    FMetaValueFactory::RegisterSupportedTypes();
    FTransformMapFactory::RegisterSupportedTypes();
}

void UVoxelDatabase::UninitializeTypes()
{
    FGridFactory::UnregisterSupportedTypes();
    FMetaValueFactory::UnregisterSupportedTypes();
    FTransformMapFactory::UnregisterSupportedTypes();
}

const TArray<FString>& UVoxelDatabase::GetRegisteredGridTypeNames() const
{
    return FGridFactory::GetRegisteredTypeNames();
}

const TArray<FString>& UVoxelDatabase::GetRegisteredMetadataTypeNames() const
{
    return FMetaValueFactory::GetRegisteredTypeNames();
}

const TArray<FString>& UVoxelDatabase::GetRegisteredTransformMapTypeNames() const
{
    return FTransformMapFactory::GetRegisteredTypeNames();
}

void UVoxelDatabase::GetGridsDisplay(TMap<FGuid, FText>& OutGridsDisplay) const
{
    for (auto i = Grids.CreateConstIterator(); i; ++i)
    {
        if (i.Value() != nullptr)
        {
            openvdb::TypedMetadata<FVoxelDatabaseTextMeta>::Ptr MetaValuePtr = i->Value->getMetadata<openvdb::TypedMetadata<FVoxelDatabaseTextMeta>>(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText));
            check(MetaValuePtr != nullptr);
            const FGuid& GridId = i.Key();
            OutGridsDisplay.Emplace(GridId, MetaValuePtr->value().Value);
        }
    }
}

void UVoxelDatabase::GetMetadataDisplay(TMap<FGuid, FText>& OutMetadataDisplay) const
{
    for (auto i = Metadata.CreateConstIterator(); i; ++i)
    {
        if (i.Value() != nullptr)
        {
            //Display the metavalue according to its string representation
            const FGuid& MetaId = i.Key();
            OutMetadataDisplay.Emplace(MetaId, FText::FromString(FString(UTF8_TO_TCHAR(i.Value()->str().c_str()))));
        }
    }
}

UVoxelDatabaseProxy* UVoxelDatabase::GetDatabaseProxy()
{
    if (VoxelDatabaseProxy && VoxelDatabaseProxy->IsValidLowLevel())
    {
        return VoxelDatabaseProxy;
    }

    VoxelDatabaseProxy = NewObject<UVoxelDatabaseProxy>(this);
    check(VoxelDatabaseProxy);

    //for (auto i = Grids.CreateConstIterator(); i; ++i)
    //{
    //    FGridFactory::ValueTypePtr GridPtr = i.Value();
    //    if (GridPtr != nullptr)
    //    {
    //        AVoxelGridProxy* GridProxy = nullptr;
    //        if (GridPtr->isType<openvdb::Grid<openvdb::tree::Tree4<bool>::Type>>())
    //        {
    //            GridProxy = NewObject<AVoxelGridProxyBool>(VoxelDatabaseProxy);
    //        }
    //        //TODO other types

    //        if (GridProxy)
    //        {
    //            GridProxy->GridId = i.Key();
    //            const openvdb::Name GridDisplayTextMetaName = TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText);
    //            openvdb::TypedMetadata<FVoxelDatabaseText>::ConstPtr GridDisplayTextMetaPtr = GridPtr->getMetadata<openvdb::TypedMetadata<FVoxelDatabaseText>>(GridDisplayTextMetaName);
    //            if (GridDisplayTextMetaPtr != nullptr)
    //            {
    //                GridProxy->GridDisplayText = GridDisplayTextMetaPtr->value().Value;
    //            }
    //            else
    //            {
    //                GridProxy->GridDisplayText = FText::FromString(FString(UTF8_TO_TCHAR(GridPtr->getName().c_str())));
    //            }

    //            VoxelDatabaseProxy->GridProxies.Add(GridProxy);
    //        }
    //    }
    //}

    return VoxelDatabaseProxy;
}
