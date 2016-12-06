#include "VoxelMatePCH.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabase.h"

//TODO idea for faster grid lookup
//TArray<FGridFactory::ValueTypeWeakPtr> CachedGrids; //Careful of anyone accessing this from other threads

void AVoxelGridProxy::ResetCoordinateTransformToAffine(const FAffineCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToUnitary(const FUnitaryCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToScale(const FScaleCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScale(const FUniformScaleCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToTranslation(const FTranslationCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToScaleTranslation(const FScaleTranslationCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToUniformScaleTranslation(const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::ResetCoordinateTransformToNonlinearFrustum(const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
{
    UVoxelDatabase::Get().SetCoordinateTransform(GridId, InCoordinateTransform);
}

void AVoxelGridProxy::PrependCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
    UVoxelDatabase::Get().GridRotation(GridId, ETransformOp::PreOp, AngleRadians, Axis);
}

void AVoxelGridProxy::PrependCoordinateTransformTranslation(const FVector& InTranslation)
{
    UVoxelDatabase::Get().GridTranslation(GridId, ETransformOp::PreOp, InTranslation);
}

void AVoxelGridProxy::PrependCoordinateTransformScale(const FVector& InScale)
{
    UVoxelDatabase::Get().GridScale(GridId, ETransformOp::PreOp, InScale);
}

void AVoxelGridProxy::PrependCoordinateTransformUniformScale(float Scale)
{
    UVoxelDatabase::Get().GridUniformScale(GridId, ETransformOp::PreOp, Scale);
}

void AVoxelGridProxy::PrependCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
    UVoxelDatabase::Get().GridShear(GridId, ETransformOp::PreOp, Shear, FirstAxis, SecondAxis);
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
    UVoxelDatabase::Get().GridMatrix4dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
}

void AVoxelGridProxy::PrependCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
    UVoxelDatabase::Get().GridMatrix3dMultiply(GridId, ETransformOp::PreOp, InFirstRow, InSecondRow, InThirdRow);
}

void AVoxelGridProxy::AppendCoordinateTransformRotation(float AngleRadians, EAxis::Type Axis)
{
    UVoxelDatabase::Get().GridRotation(GridId, ETransformOp::PostOp, AngleRadians, Axis);
}

void AVoxelGridProxy::AppendCoordinateTransformTranslation(const FVector& InTranslation)
{
    UVoxelDatabase::Get().GridTranslation(GridId, ETransformOp::PostOp, InTranslation);
}

void AVoxelGridProxy::AppendCoordinateTransformScale(const FVector& InScale)
{
    UVoxelDatabase::Get().GridScale(GridId, ETransformOp::PostOp, InScale);
}

void AVoxelGridProxy::AppendCoordinateTransformUniformScale(float Scale)
{
    UVoxelDatabase::Get().GridUniformScale(GridId, ETransformOp::PostOp, Scale);
}

void AVoxelGridProxy::AppendCoordinateTransformShear(float Shear, EAxis::Type FirstAxis, EAxis::Type SecondAxis)
{
    UVoxelDatabase::Get().GridShear(GridId, ETransformOp::PostOp, Shear, FirstAxis, SecondAxis);
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix4dMultiply(const FPlane &InFirstRow, const FPlane &InSecondRow, const FPlane &InThirdRow, const FPlane &InFourthRow)
{
    UVoxelDatabase::Get().GridMatrix4dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow, InFourthRow);
}

void AVoxelGridProxy::AppendCoordinateTransformMatrix3dMultiply(const FVector &InFirstRow, const FVector &InSecondRow, const FVector &InThirdRow)
{
    UVoxelDatabase::Get().GridMatrix3dMultiply(GridId, ETransformOp::PostOp, InFirstRow, InSecondRow, InThirdRow);
}

bool AVoxelGridProxy::LoadVoxelData()
{
    return false;
}

bool AVoxelGridProxy::SaveVoxelData()
{
    return false;
}

//TODO Get/SetVoxelValue from VoxelDatabaseProxy
#define GRID_PROXY_IMPLEMENTATION(ValueType, Name)\
const ValueType& AVoxelGridProxy##Name::GetVoxelValue(const FIntVector& IndexCoord) const\
{\
    return UVoxelDatabase::Get().GetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord).Value;\
}\
void AVoxelGridProxy##Name::GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const\
{\
    UVoxelDatabase::Get().GetVoxelIsActive<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, OutIsActive);\
}\
const ValueType& AVoxelGridProxy##Name::GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const\
{\
    return UVoxelDatabase::Get().GetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, OutIsActive).Value;\
}\
void AVoxelGridProxy##Name::SetVoxelValue(const FIntVector& IndexCoord, const ValueType& InValue)\
{\
    UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, InValue);\
}\
void AVoxelGridProxy##Name::SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive)\
{\
    UVoxelDatabase::Get().SetVoxelIsActive<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, InIsActive);\
}\
void AVoxelGridProxy##Name::SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const ValueType& InValue, const bool& InIsActive)\
{\
    UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, InValue, InIsActive);\
}\
void AVoxelGridProxy##Name::FillValue(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const ValueType& InValue, const bool& InIsActive, const bool& InVoxelizeActiveTilesAfterFill)\
{\
    UVoxelDatabase::Get().FillGrid<FVoxelDatabase##Name##Voxel>(GridId, StartIndexCoord, FillDimensions, InValue, InIsActive, InVoxelizeActiveTilesAfterFill);\
}\
void AVoxelGridProxy##Name::SetValues(EVoxelIterator VoxelIter, const TScriptInterface<IVoxel##Name##SourceInterface>& ValueSourceInterface)\
{\
    if(VoxelIter == EVoxelIterator::InactiveVoxelsIter)\
    {\
        UVoxelDatabase::Get().SetVoxelValuesFromSource<FVoxelDatabase##Name##Voxel, IVoxel##Name##SourceInterface, EVoxelIterator::InactiveVoxelsIter>(GridId, ValueSourceInterface);\
    }\
    else if (VoxelIter == EVoxelIterator::ActiveVoxelsIter)\
    {\
        UVoxelDatabase::Get().SetVoxelValuesFromSource<FVoxelDatabase##Name##Voxel, IVoxel##Name##SourceInterface, EVoxelIterator::ActiveVoxelsIter>(GridId, ValueSourceInterface);\
    }\
    else\
    {\
        UVoxelDatabase::Get().SetVoxelValuesFromSource<FVoxelDatabase##Name##Voxel, IVoxel##Name##SourceInterface, EVoxelIterator::AllVoxelsIter>(GridId, ValueSourceInterface);\
    }\
}

GRID_PROXY_IMPLEMENTATION(bool, Bool)
GRID_PROXY_IMPLEMENTATION(double, Double)
GRID_PROXY_IMPLEMENTATION(float, Float)
GRID_PROXY_IMPLEMENTATION(int8, Int8)
GRID_PROXY_IMPLEMENTATION(int16, Int16)
GRID_PROXY_IMPLEMENTATION(int32, Int32)
GRID_PROXY_IMPLEMENTATION(int64, Int64)
GRID_PROXY_IMPLEMENTATION(uint8, UInt8)
GRID_PROXY_IMPLEMENTATION(uint16, UInt16)
GRID_PROXY_IMPLEMENTATION(uint32, UInt32)
GRID_PROXY_IMPLEMENTATION(uint64, UInt64)
GRID_PROXY_IMPLEMENTATION(FColor, Color)
GRID_PROXY_IMPLEMENTATION(FLinearColor, LinearColor)
GRID_PROXY_IMPLEMENTATION(FPackedNormal, PackedNormal)
GRID_PROXY_IMPLEMENTATION(FPackedRGB10A2N, PackedRGB10A2N)
GRID_PROXY_IMPLEMENTATION(FPackedRGBA16N, PackedRGBA16N)
GRID_PROXY_IMPLEMENTATION(FVector, Vector)
GRID_PROXY_IMPLEMENTATION(FVector2D, Vector2D)
GRID_PROXY_IMPLEMENTATION(FVector4, Vector4)
GRID_PROXY_IMPLEMENTATION(FIntPoint, IntPoint)
GRID_PROXY_IMPLEMENTATION(FIntVector, IntVector)
