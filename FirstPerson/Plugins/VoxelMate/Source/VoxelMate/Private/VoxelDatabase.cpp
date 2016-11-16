#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "VoxelDatabase.h"
#include "ArchiveTransformMap.h"
#include "VoxelGridProxyBool.h"

void UVoxelDatabase::InitializeTypes()
{
    if (IsDatabaseInitialized)
    {
        return;
    }

    //TODO: Note that the following BLOSC related stuff is in openvdb::initialize
//#ifdef OPENVDB_USE_BLOSC
//    blosc_init();
//    if (blosc_set_compressor("lz4") < 0) {
//        OPENVDB_LOG_WARN("Blosc LZ4 compressor is unavailable");
//    }
//    /// @todo blosc_set_nthreads(int nthreads);
//#endif

    //Register the grid types per supported type and tree
    FGridFactory::ClearRegistry();
    FGridFactory::RegisterGridType<bool>();
    FGridFactory::RegisterGridType<float>();
    FGridFactory::RegisterGridType<double>();
    FGridFactory::RegisterGridType<uint8>();
    FGridFactory::RegisterGridType<uint16>();
    FGridFactory::RegisterGridType<uint32>();
    FGridFactory::RegisterGridType<uint64>();
    FGridFactory::RegisterGridType<int8>();
    FGridFactory::RegisterGridType<int16>();
    FGridFactory::RegisterGridType<int32>();
    FGridFactory::RegisterGridType<int64>();
    FGridFactory::RegisterGridType<FVoxelDatabaseVector>();
    FGridFactory::RegisterGridType<FVoxelDatabaseVector4>();
    FGridFactory::RegisterGridType<FVoxelDatabaseVector2D>();
    FGridFactory::RegisterGridType<FVoxelDatabaseColor>();
    FGridFactory::RegisterGridType<FVoxelDatabaseLinearColor>();
    FGridFactory::RegisterGridType<FVoxelDatabasePackedNormal>();
    FGridFactory::RegisterGridType<FVoxelDatabasePackedRGB10A2N>();
    FGridFactory::RegisterGridType<FVoxelDatabasePackedRGBA16N>();

    //Register metadata types per supported type
    FMetaValueFactory::ClearRegistry();
    FMetaValueFactory::RegisterMetaType<bool>();
    FMetaValueFactory::RegisterMetaType<float>();
    FMetaValueFactory::RegisterMetaType<double>();
    FMetaValueFactory::RegisterMetaType<uint8>();
    FMetaValueFactory::RegisterMetaType<uint16>();
    FMetaValueFactory::RegisterMetaType<uint32>();
    FMetaValueFactory::RegisterMetaType<uint64>();
    FMetaValueFactory::RegisterMetaType<int8>();
    FMetaValueFactory::RegisterMetaType<int16>();
    FMetaValueFactory::RegisterMetaType<int32>();
    FMetaValueFactory::RegisterMetaType<int64>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseVector>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseVector4>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseVector2D>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseColor>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseLinearColor>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabasePackedNormal>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabasePackedRGB10A2N>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabasePackedRGBA16N>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseIntPoint>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseIntVector>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseFloatRangeBound>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseFloatRange>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInt32RangeBound>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInt32Range>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseFloatInterval>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInt32Interval>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseTwoVectors>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabasePlane>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseRotator>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseQuat>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseBox>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseBox2D>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseBoxSphereBounds>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseOrientedBox>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseMatrix>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointFloat>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveFloat>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointVector2D>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveVector2D>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointVector>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveVector>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointQuat>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveQuat>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointTwoVectors>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveTwoVectors>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurvePointLinearColor>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseInterpCurveLinearColor>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseTransform>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseRandomStream>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseDateTime>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseTimespan>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseStringAssetReference>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseStringClassReference>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseString>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseName>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseText>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseAffineMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseUnitaryMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseScaleMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseUniformScaleMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseTranslationMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseScaleTranslationMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseUniformScaleTranslationMap>();
    FMetaValueFactory::RegisterMetaType<FVoxelDatabaseNonlinearFrustumMap>();

    //Register transform map types per supported type
    FTransformMapFactory::ClearRegistry();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::AffineMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::UnitaryMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::ScaleMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::UniformScaleMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::TranslationMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::ScaleTranslateMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::UniformScaleTranslateMap>();
    FTransformMapFactory::RegisterTransformMapType<openvdb::math::NonlinearFrustumMap>();

    IsDatabaseInitialized = true;
}

void UVoxelDatabase::UninitializeTypes()
{
    FGridFactory::ClearRegistry();
    FMetaValueFactory::ClearRegistry();
    FTransformMapFactory::ClearRegistry();
    IsDatabaseInitialized = false;
}

const TArray<FString>& UVoxelDatabase::GetRegisteredGridTypeNames() const
{
    return FGridFactory::RegisteredTypeNames;
}

const TArray<FString>& UVoxelDatabase::GetRegisteredMetadataTypeNames() const
{
    return FMetaValueFactory::RegisteredTypeNames;
}

const TArray<FString>& UVoxelDatabase::GetRegisteredTransformMapTypeNames() const
{
    return FTransformMapFactory::RegisteredTypeNames;
}

void UVoxelDatabase::GetGridsDisplay(TMap<FGuid, FText>& OutGridsDisplay) const
{
    for (auto i = Grids.CreateConstIterator(); i; ++i)
    {
        if (i.Value() != nullptr)
        {
            openvdb::TypedMetadata<FVoxelDatabaseText>::Ptr MetaValuePtr = i->Value->getMetadata<openvdb::TypedMetadata<FVoxelDatabaseText>>(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText));
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

bool UVoxelDatabase::AddMetadata(const FString& TypeName, FGuid& OutMetadataId)
{
    bool IsMetadataAdded = false;
    FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create(TypeName);
    if (MetaPtr != nullptr)
    {
        OutMetadataId = FGuid::NewGuid();
        check(!Metadata.Contains(OutMetadataId));
        Metadata.Add(OutMetadataId, MetaPtr);
        IsMetadataAdded = true;
    }
    return IsMetadataAdded;
}
