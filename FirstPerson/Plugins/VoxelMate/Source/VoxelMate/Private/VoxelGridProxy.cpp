#include "VoxelMatePCH.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"

#include "FastNoiseSIMD.h"
#undef FN_ALIGNED_SETS //TOOD Need to determine how aligned noise sets will interact with TSharedPtr

//TODO idea for faster grid lookup
//TArray<FGridFactory::ValueTypeWeakPtr> CachedGrids; //Careful of anyone accessing this from other threads

struct FIntBox
{
    FIntVector Start;
    FIntVector Size;

    friend uint32 GetTypeHash(const FIntBox& IntBox)
    {
        return HashCombine(GetTypeHash(IntBox.Start), GetTypeHash(IntBox.Size));
    }

    friend bool operator==(const FIntBox& InLhs, const FIntBox& InRhs)
    {
        return InLhs.Start == InRhs.Start && InLhs.Size == InRhs.Size;
    }
};

struct FastNoiseSIMDSet
{
    TSharedPtr<FastNoiseSIMD> NoiseModule;
    TMap<FIntBox, TSharedPtr<float>> GeneratedValues;
};

TMap<FNoiseGeneratorConfiguration, FastNoiseSIMDSet> FastNoiseSIMDSets;

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
    FastNoiseSIMDSet& NoiseSet = FastNoiseSIMDSets.FindOrAdd(Config);
    if (!NoiseSet.NoiseModule.IsValid())
    {
        NoiseSet.NoiseModule = TSharedPtr<FastNoiseSIMD>(FastNoiseSIMD::NewFastNoiseSIMD(Config.RandomSeed));
        NoiseSet.NoiseModule->SetFrequency(Config.Frequency);        
        NoiseSet.NoiseModule->SetAxisScales(Config.AxisScale.X, Config.AxisScale.Y, Config.AxisScale.Z);
        NoiseSet.NoiseModule->SetFractalOctaves(Config.OctaveCount);
        NoiseSet.NoiseModule->SetFractalLacunarity(Config.Lacunarity);
        NoiseSet.NoiseModule->SetFractalGain(Config.Gain);

        switch (Config.NoiseType)
        {
        case ENoiseType::Value:
            NoiseSet.NoiseModule->SetNoiseType(Config.FractalMode == ENoiseFractalMode::None ? FastNoiseSIMD::NoiseType::Value : FastNoiseSIMD::NoiseType::ValueFractal);
            break;
        case ENoiseType::Perlin:
            NoiseSet.NoiseModule->SetNoiseType(Config.FractalMode == ENoiseFractalMode::None ? FastNoiseSIMD::NoiseType::Perlin : FastNoiseSIMD::NoiseType::PerlinFractal);
            break;
        case ENoiseType::Simplex:
            NoiseSet.NoiseModule->SetNoiseType(Config.FractalMode == ENoiseFractalMode::None ? FastNoiseSIMD::NoiseType::Simplex : FastNoiseSIMD::NoiseType::SimplexFractal);
            break;
        case ENoiseType::White:
            NoiseSet.NoiseModule->SetNoiseType(FastNoiseSIMD::NoiseType::WhiteNoise);
            break;
        case ENoiseType::Cellular:
            NoiseSet.NoiseModule->SetNoiseType(FastNoiseSIMD::NoiseType::Cellular);
            break;
        default:
            //TODO log error (unexpected noise type)
            NoiseSet.NoiseModule->SetNoiseType(Config.FractalMode == ENoiseFractalMode::None ? FastNoiseSIMD::NoiseType::Value : FastNoiseSIMD::NoiseType::ValueFractal);
            break;
        }

        switch (Config.FractalMode)
        {
        case ENoiseFractalMode::None:
            NoiseSet.NoiseModule->SetFractalType(FastNoiseSIMD::FractalType::FBM);
            break;
        case ENoiseFractalMode::FractionalBrownianMotion:
            NoiseSet.NoiseModule->SetFractalType(FastNoiseSIMD::FractalType::FBM);
            break;
        case ENoiseFractalMode::Billow:
            NoiseSet.NoiseModule->SetFractalType(FastNoiseSIMD::FractalType::Billow);
            break;
        case ENoiseFractalMode::RigidMulti:
            NoiseSet.NoiseModule->SetFractalType(FastNoiseSIMD::FractalType::RigidMulti);
            break;
        default:
            //TODO log error (unexpected noise type)
            NoiseSet.NoiseModule->SetFractalType(FastNoiseSIMD::FractalType::FBM);
            break;
        }

        switch (Config.CellularDistanceMode)
        {
        case ENoiseCellularDistanceMode::Euclidean:
            NoiseSet.NoiseModule->SetCellularDistanceFunction(FastNoiseSIMD::CellularDistanceFunction::Euclidean);
            break;
        case ENoiseCellularDistanceMode::Manhattan:
            NoiseSet.NoiseModule->SetCellularDistanceFunction(FastNoiseSIMD::CellularDistanceFunction::Manhattan);
            break;
        case ENoiseCellularDistanceMode::Natural:
            NoiseSet.NoiseModule->SetCellularDistanceFunction(FastNoiseSIMD::CellularDistanceFunction::Natural);
            break;
        default:
            //TODO log error (unexpected noise type)
            NoiseSet.NoiseModule->SetCellularDistanceFunction(FastNoiseSIMD::CellularDistanceFunction::Euclidean);
            break;
        }

        switch (Config.CellularReturnType)
        {
        case ENoiseCellularReturnType::CellValue:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::CellValue);
            break;
        case ENoiseCellularReturnType::Distance:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance);
            break;
        case ENoiseCellularReturnType::Distance2:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance2);
            break;
        case ENoiseCellularReturnType::Distance2Add:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance2Add);
            break;
        case ENoiseCellularReturnType::Distance2Sub:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance2Sub);
            break;
        case ENoiseCellularReturnType::Distance2Mul:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance2Mul);
            break;
        case ENoiseCellularReturnType::Distance2Div:
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance2Div);
            break;
        default:
            //TODO log error (unexpected noise type)
            NoiseSet.NoiseModule->SetCellularReturnType(FastNoiseSIMD::CellularReturnType::Distance);
            break;
        }
    }

    FIntBox Box;
    Box.Start = StartIndexCoord;
    Box.Size = FillDimensions;

    TSharedPtr<float> Values;
    TSharedPtr<float>* FindValues = NoiseSet.GeneratedValues.Find(Box);
    if (!FindValues)
    {
        Values = NoiseSet.GeneratedValues.Add(Box, TSharedPtr<float>(NoiseSet.NoiseModule->GetNoiseSet(Box.Start.X, Box.Start.Y, Box.Start.Z, Box.Size.X, Box.Size.Y, Box.Size.Z)));
    }
    else
    {
        Values = *FindValues;
    }

    float* ValuesPtr = Values.Get();
    TArray<TArray<TArrayView<float>>> Noise3DValues;
    Noise3DValues.Reserve(Box.Size.X);
    for (int32 x = 0; x < Box.Size.X; ++x)
    {
        const int32 yarray = Noise3DValues.Add(TArray<TArrayView<float>>());
        Noise3DValues[yarray].Reserve(Box.Size.Y);
        for (int32 y = 0; y < Box.Size.Y; ++y)
        {
            float* zblock = ValuesPtr + x + Box.Size.X * (y + Box.Size.Y);
            const int32 zarray = Noise3DValues[yarray].Add(TArrayView<float>(zblock, Box.Size.Z));
        }
    }
    typedef NoiseValueOp<openvdb::Grid<openvdb::tree::Tree4<FVoxelDatabaseFloatVoxel>::Type>::ValueOnIter, FVoxelDatabaseFloatVoxel> NoiseValueOpType;
    NoiseValueOpType ValueOp(Noise3DValues);
    UVoxelDatabase::Get().RunGridOp<NoiseValueOpType>(GridId, ValueOp);
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
