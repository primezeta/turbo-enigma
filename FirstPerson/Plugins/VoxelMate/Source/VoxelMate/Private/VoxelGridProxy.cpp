#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"

//TODO idea for faster grid lookup
//TArray<FGridFactory::ValueTypeWeakPtr> CachedGrids; //Careful of anyone accessing this from other threads

AVoxelGridProxy::AVoxelGridProxy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), IsFloatSavedAsHalf(false)//, CacheIndex(-1) TODO idea for faster grid lookup
{
    GridMeshComponent = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("GridMeshComponent"));
    IsFloatSavedAsHalf = false;

    FString DefaultGridDisplayText;
    GetName(DefaultGridDisplayText);
    GridDisplayText = FText::FromString(DefaultGridDisplayText);

    DataFilePath = TEXT("");
    //TODO idea for faster grid lookup
    //UVoxelDatabase::Get().Grids
    //CacheIndex = 
}

FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy)
{
    if (!GridProxy.IsDefaultSubobject())
    {
        Ar << GridProxy.GridId;
        Ar << GridProxy.IsFloatSavedAsHalf;
        Ar << GridProxy.GridDisplayText;
        Ar << GridProxy.DataFilePath;
    }
    return Ar;
}

void AVoxelGridProxy::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    Ar << *this;
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
void AVoxelGridProxy##Name::Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const ValueType& InValue, const bool& InIsActive)\
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
