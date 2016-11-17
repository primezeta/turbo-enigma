#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"
#include "VoxelGridProxy.h"
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
#include "VoxelGridProxyIntPoint.h"
#include "VoxelGridProxyIntVector.h"

FArchive& operator<<(FArchive& Ar, UVoxelDatabaseProxy& DatabaseProxy)
{
    if (!DatabaseProxy.IsDefaultSubobject())
    {
        Ar << DatabaseProxy.GridProxies;
    }
    return Ar;
}

void UVoxelDatabaseProxy::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);
    Ar << *this;
}

void UVoxelDatabaseProxy::PostInitProperties()
{
    Super::PostInitProperties();
}

bool UVoxelDatabaseProxy::IsReadyForFinishDestroy()
{
    //TODO: Return true when UVoxelDatabase async serialization is complete
    return Super::IsReadyForFinishDestroy();
}

void UVoxelDatabaseProxy::BeginDestroy()
{
    //TODO: Serialize UVoxelDatabase asyncronously
    Super::BeginDestroy();
}

void UVoxelDatabaseProxy::PreSave(const class ITargetPlatform* TargetPlatform)
{
    Super::PreSave(TargetPlatform);
}

void UVoxelDatabaseProxy::PostLoad()
{
    Super::PostLoad();
}

UVoxelDatabaseProxy* UVoxelDatabaseProxy::OpenDatabaseProxy()
{
    return UVoxelDatabase::Get().GetDatabaseProxy();
}

AVoxelGridProxy* UVoxelDatabaseProxy::AddGridProxy(EVoxelDatabaseVoxelType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    FGuid GridId;
    AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelDatabaseVoxelType::Bool && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseBool>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyBool>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Float && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseFloat>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyFloat>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Double && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseDouble>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyDouble>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt8>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt8>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt16>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt16>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt32>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt32>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt64>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt64>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt8>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt8>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt16>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt16>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt32>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt32>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt64>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt64>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector4 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector4>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector4>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector2D && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector2D>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector2D>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Color && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseColor>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyColor>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::LinearColor && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseLinearColor>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyLinearColor>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedNormal && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedNormal>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedNormal>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedRGB10A2N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGB10A2N>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGB10A2N>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedRGBA16N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGBA16N>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGBA16N>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::IntPoint && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntPoint>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntPoint>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::IntVector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntVector>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntVector>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }

    if (GridProxy)
    {
        check(GridId.IsValid());
        GridProxy->GridId = GridId;
        GridProxy->IsFloatSavedAsHalf = SaveFloatAsHalf;
        GridProxy->GridDisplayText = GridDisplayText;
        GridProxies.Add(GridProxy);
    }
    return GridProxy;
}
