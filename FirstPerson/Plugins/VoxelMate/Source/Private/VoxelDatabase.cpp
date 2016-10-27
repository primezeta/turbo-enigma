#include "VoxelMatePrivatePCH.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabase.h"

void UVoxelDatabase::InitializeStandardTypes()
{
    //Register the standard grid and metadata types
    FGridFactory::ClearRegistry();
    FGridFactory::RegisterStandardGrid<bool>();
    FGridFactory::RegisterStandardGrid<float>();
    FGridFactory::RegisterStandardGrid<double>();
    FGridFactory::RegisterStandardGrid<int32>();
    FGridFactory::RegisterStandardGrid<int64>();
    FGridFactory::RegisterStandardGrid<FVector2D>();
    FGridFactory::RegisterStandardGrid<FVector>();
    FGridFactory::RegisterStandardGrid<FVector4>();
    FGridFactory::RegisterStandardGrid<FIntVector2>();
    FGridFactory::RegisterStandardGrid<FIntVector>();
    FGridFactory::RegisterStandardGrid<FIntVector4>();
    FGridFactory::RegisterStandardGrid<FUintVector4>();
    FGridFactory::RegisterStandardGrid<FPointIndex32>();
    FGridFactory::RegisterStandardGrid<FPointIndex64>();
    FGridFactory::RegisterStandardGrid<openvdb::Name>(); //TODO register FString instead?

    FMetaValueFactory::ClearRegistry();
    FMetaValueFactory::RegisterMetaType<bool>();
    FMetaValueFactory::RegisterMetaType<double>();
    FMetaValueFactory::RegisterMetaType<float>();
    FMetaValueFactory::RegisterMetaType<int32>();
    FMetaValueFactory::RegisterMetaType<int64>();
    FMetaValueFactory::RegisterMetaType<FIntVector2>();
    FMetaValueFactory::RegisterMetaType<FVector2D>();
    FMetaValueFactory::RegisterMetaType<FIntVector>();
    FMetaValueFactory::RegisterMetaType<FVector>();
    FMetaValueFactory::RegisterMetaType<FMatrix>();
    FMetaValueFactory::RegisterMetaType<FPointIndex32>();
    FMetaValueFactory::RegisterMetaType<FPointIndex64>();
    FMetaValueFactory::RegisterMetaType<openvdb::Name>();

    FTransformMapFactory::ClearRegistry();
    FTransformMapFactory::RegisterTransformMapType<FTransformAffineMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformUnitaryMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformScaleMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformUniformScaleMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformTranslationMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformScaleTranslateMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformUniformScaleTranslateMap>();
    FTransformMapFactory::RegisterTransformMapType<FTransformNonlinearFrustumMap>();
}
