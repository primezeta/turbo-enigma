#include "VoxelMatePCH.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"

#include "noise.h"

//TODO idea for faster grid lookup
//TArray<FGridFactory::ValueTypeWeakPtr> CachedGrids; //Careful of anyone accessing this from other threads

struct FIntBox
{
    FIntVector Start;
    FIntVector Size;

    FIntBox(const FIntVector& InStart, const FIntVector& InSize)
        : Start(InStart), Size(InSize)
    {}

    friend uint32 GetTypeHash(const FIntBox& IntBox)
    {
        return HashCombine(GetTypeHash(IntBox.Start), GetTypeHash(IntBox.Size));
    }

    friend bool operator==(const FIntBox& InLhs, const FIntBox& InRhs)
    {
        return InLhs.Start == InRhs.Start && InLhs.Size == InRhs.Size;
    }
};

struct NoiseSet
{
    TSharedPtr<noise::module::Module> NoiseModule;
};

TMap<FNoiseGeneratorConfiguration, NoiseSet> NoiseSets;

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

void AVoxelGridProxyFloat::FillNoise(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FNoiseGeneratorConfiguration& Config, const bool& InIsActive)
{
    ////Associate each particular noise configuration to a cached set of noise values
    //NoiseSet& NoiseSet = NoiseSets.FindOrAdd(Config);
    //if (!NoiseSet.NoiseModule.IsValid())
    //{
    //    //This particular noise set has not yet been created so create it using the specified configuration
    //    NoiseSet.NoiseModule = TSharedPtr<noise::module::Module>(TSharedPtr<noise::module::Module>(new noise::module::Perlin()));
    //    check(Config.NoiseType == ENoiseType::Perlin);
    //    NoiseSet.NoiseModule->SetSeed(Config.RandomSeed);
    //    NoiseSet.NoiseModule->SetFrequency(Config.Frequency);
    //    NoiseSet.NoiseModule->SetOctaveCount(Config.OctaveCount);
    //    NoiseSet.NoiseModule->SetLacunarity(Config.Lacunarity);
    //    NoiseSet.NoiseModule->SetPersistence(Config.Gain);
    //}

    ////Check if a set of noise values was already generated from the given noise configuration within the specified volume
    //const openvdb::Coord StartCoord(StartIndexCoord.X, StartIndexCoord.Y, StartIndexCoord.Z);
    //const openvdb::Coord EndCoord(StartIndexCoord.X + FillDimensions.X - 1, StartIndexCoord.Y + FillDimensions.Y - 1, StartIndexCoord.Z + FillDimensions.Z - 1);
    //openvdb::CoordBBox NoiseFillVolume(StartCoord, EndCoord);

    ////Expand the fill-volume by 1 voxel on all sides so that a noise value is always available when checking adjacent voxels
    //NoiseFillVolume.expand((int32)1);

    //const openvdb::Coord& VolumeExtents = NoiseFillVolume.extents();

    ////Associate existing values to the specified fill dimensions, not the expanded dimensions
    //const FIntBox Volume(StartIndexCoord, FillDimensions);
    //TSharedPtr<float> Values;
    //TArray<float>* FindValues = NoiseSet.GeneratedValues.Find(Volume);
    //if (!FindValues)
    //{
    //    TArray<float>& Values = NoiseSet.GeneratedValues.Add(Volume, TArray<float>());
    //    Values.Reserve(NoiseFillVolume.volume());
    //    const int32 Width = NoiseFillVolume.dim().asVec3i().x();
    //    const int32 Depth = NoiseFillVolume.dim().asVec3i().y();
    //    for (auto x = NoiseFillVolume.min().x(); x <= NoiseFillVolume.max().x(); ++x)
    //    {
    //        for (auto y = NoiseFillVolume.min().y(); y <= NoiseFillVolume.max().y(); ++y)
    //        {
    //            for (auto z = NoiseFillVolume.min().z(); z <= NoiseFillVolume.max().z(); ++z)
    //            {
    //                const int32 ValueIndex = x + Width * (y + Depth*z);
    //                Values[ValueIndex] = NoiseSet.NoiseModule->GetValue();
    //            }
    //        }
    //    }
    //    //Values for these dimensions have not yet been generated, generate them
    //    Values = NoiseSet.GeneratedValues.Add(Volume, TSharedPtr<float>(NoiseSet.NoiseModule->));
    //}
    //else
    //{
    //    //Values for these dimensions were already generated, use those
    //    Values = *FindValues;
    //}

    ////
    //float* ValuesPtr = Values.Get();
    //TArray<TArray<TArrayView<float>>> Noise3DValues;
    //Noise3DValues.Reserve(VolumeExtents.x());
    //for (int32 x = 0; x < VolumeExtents.x(); ++x)
    //{
    //    const int32 yarray = Noise3DValues.Add(TArray<TArrayView<float>>());
    //    Noise3DValues[yarray].Reserve(VolumeExtents.y());
    //    for (int32 y = 0; y < VolumeExtents.y(); ++y)
    //    {
    //        float* zblock = ValuesPtr + x + VolumeExtents.x() * (y + VolumeExtents.y());
    //        const int32 zarray = Noise3DValues[yarray].Add(TArrayView<float>(zblock, VolumeExtents.z()));
    //    }
    //}

    //typedef NoiseValueOp<openvdb::Grid<openvdb::tree::Tree4<FVoxelDatabaseFloatVoxel>::Type>::ValueOnIter, FVoxelDatabaseFloatVoxel> NoiseValueOpType;
    //NoiseValueOpType ValueOp(Noise3DValues);
    //UVoxelDatabase::Get().RunGridOp<NoiseValueOpType>(GridId, ValueOp);
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
void AVoxelGridProxy##Name::FillValue(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const ValueType& InValue, const bool& InIsActive)\
{\
    UVoxelDatabase::Get().FillGrid<FVoxelDatabase##Name##Voxel>(GridId, StartIndexCoord, FillDimensions, InValue, InIsActive);\
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
