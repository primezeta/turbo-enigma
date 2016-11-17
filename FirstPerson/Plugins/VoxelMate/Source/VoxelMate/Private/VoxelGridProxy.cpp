#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"
#include "VoxelGridProxyBool.h"
#include "VoxelGridProxyDouble.h"
#include "VoxelGridProxyFloat.h"
#include "VoxelGridProxyInt8.h"
#include "VoxelGridProxyInt16.h"
#include "VoxelGridProxyInt32.h"
#include "VoxelGridProxyInt64.h"
#include "VoxelGridProxyUInt8.h"
#include "VoxelGridProxyUInt16.h"
#include "VoxelGridProxyUInt32.h"
#include "VoxelGridProxyUInt64.h"
#include "VoxelGridProxyColor.h"
#include "VoxelGridProxyLinearColor.h"
#include "VoxelGridProxyPackedNormal.h"
#include "VoxelGridProxyPackedRGB10A2N.h"
#include "VoxelGridProxyPackedRGBA16N.h"
#include "VoxelGridProxyVector.h"
#include "VoxelGridProxyVector2D.h"
#include "VoxelGridProxyVector4.h"

#define GRID_PROXY_IMPLEMENTATION(ValueType, Name)\
const ValueType& AVoxelGridProxy##Name::GetVoxelValue(const FIntVector& IndexCoord)\
{\
    return UVoxelDatabase::Get().GetVoxelValue<FVoxelDatabase##Name>(GridId, IndexCoord).Value;\
}\
\
const bool AVoxelGridProxy##Name::GetVoxelIsActive(const FIntVector& IndexCoord)\
{\
    return UVoxelDatabase::Get().GetVoxelIsActive<FVoxelDatabase##Name>(GridId, IndexCoord);\
}\
\
const ValueType& AVoxelGridProxy##Name::GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive)\
{\
    return UVoxelDatabase::Get().GetVoxelValue<FVoxelDatabase##Name>(GridId, IndexCoord, OutIsActive).Value;\
}\
\
void AVoxelGridProxy##Name::SetVoxelValue(const FIntVector& IndexCoord, const ValueType& InValue)\
{\
    UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name>(GridId, IndexCoord, FVoxelDatabase##Name(InValue));\
}\
\
void AVoxelGridProxy##Name::SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive)\
{\
    UVoxelDatabase::Get().SetVoxelIsActive<FVoxelDatabase##Name>(GridId, IndexCoord, InIsActive);\
}\
\
void AVoxelGridProxy##Name::SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const ValueType& InValue, const bool& InIsActive)\
{\
    UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name>(GridId, IndexCoord, FVoxelDatabase##Name(InValue), InIsActive);\
}

FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy)
{
    if (!GridProxy.IsDefaultSubobject())
    {
        Ar << GridProxy.GridId;
        Ar << GridProxy.GridDisplayText;
    }
    return Ar;
}

void AVoxelGridProxy::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    Ar << *this;
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
