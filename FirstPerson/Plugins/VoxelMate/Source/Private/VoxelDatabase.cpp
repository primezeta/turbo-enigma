#include "VoxelMatePrivatePCH.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabase.h"

void UVoxelDatabase::InitializeGridTypes()
{
    //Register the grid types per supported type and tree
    FGridFactory::ClearRegistry();
    FGridFactory::RegisterGrid3<bool>();
    FGridFactory::RegisterGrid4<bool>();
    FGridFactory::RegisterGrid5<bool>();
    FGridFactory::RegisterGrid3<float>();
    FGridFactory::RegisterGrid4<float>();
    FGridFactory::RegisterGrid5<float>();
    FGridFactory::RegisterGrid3<double>();
    FGridFactory::RegisterGrid4<double>();
    FGridFactory::RegisterGrid5<double>();
    FGridFactory::RegisterGrid3<int32>();
    FGridFactory::RegisterGrid4<int32>();
    FGridFactory::RegisterGrid5<int32>();
    FGridFactory::RegisterGrid3<int64>();
    FGridFactory::RegisterGrid4<int64>();
    FGridFactory::RegisterGrid5<int64>();
    FGridFactory::RegisterGrid3<FVector2D>();
    FGridFactory::RegisterGrid4<FVector2D>();
    FGridFactory::RegisterGrid5<FVector2D>();
    FGridFactory::RegisterGrid3<FVector>();
    FGridFactory::RegisterGrid4<FVector>();
    FGridFactory::RegisterGrid5<FVector>();
    FGridFactory::RegisterGrid3<FVector4>();
    FGridFactory::RegisterGrid4<FVector4>();
    FGridFactory::RegisterGrid5<FVector4>();
    FGridFactory::RegisterGrid3<FIntVector2>();
    FGridFactory::RegisterGrid4<FIntVector2>();
    FGridFactory::RegisterGrid5<FIntVector2>();
    FGridFactory::RegisterGrid3<FIntVector>();
    FGridFactory::RegisterGrid4<FIntVector>();
    FGridFactory::RegisterGrid5<FIntVector>();
    FGridFactory::RegisterGrid3<FIntVector4>();
    FGridFactory::RegisterGrid4<FIntVector4>();
    FGridFactory::RegisterGrid5<FIntVector4>();
    FGridFactory::RegisterGrid3<FUintVector4>();
    FGridFactory::RegisterGrid4<FUintVector4>();
    FGridFactory::RegisterGrid5<FUintVector4>();
    FGridFactory::RegisterGrid3<FPointIndex32>();
    FGridFactory::RegisterGrid4<FPointIndex32>();
    FGridFactory::RegisterGrid5<FPointIndex32>();
    FGridFactory::RegisterGrid3<FPointIndex64>();
    FGridFactory::RegisterGrid4<FPointIndex64>();
    FGridFactory::RegisterGrid5<FPointIndex64>();

    //Register metadata types per supported type
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
    FMetaValueFactory::RegisterMetaType<FString>();

    //Register transform map types per supported type
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
