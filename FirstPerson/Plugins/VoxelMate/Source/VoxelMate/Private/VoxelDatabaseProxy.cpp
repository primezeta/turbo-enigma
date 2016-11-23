#include "VoxelMatePCH.h"
#include "VoxelDatabaseCommonPrivate.h"
#include "VoxelDatabase.h"
#include "VoxelGridProxy.h"

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

UVoxelDatabaseProxy::UVoxelDatabaseProxy()
{
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
    UVoxelDatabaseProxy* VoxelDatabaseProxy = nullptr;
    if (FVoxelMateModule::IsAvailable())
    {
        VoxelDatabaseProxy = UVoxelDatabase::Get().GetDatabaseProxy();
    }
    return VoxelDatabaseProxy;
}

bool UVoxelDatabaseProxy::CreateGridDataFile(AVoxelGridProxy* GridProxy, FText& OutFailureReason)
{
    if (!GridProxy)
    {
        OutFailureReason.FromString(FString(TEXT("Invalid grid proxy")));
        return false;
    }

    if (!GridProxy->GridId.IsValid())
    {
        OutFailureReason.FromString(FString(TEXT("Invalid grid proxy ID")));
        return false;
    }

    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (FPaths::FileExists(GridProxy->DataFilePath))
    {
        check(FPaths::ValidatePath(GridProxy->DataFilePath));
        return true;
    }

    const FString& DataPath = UVoxelDatabase::Get().DataPath;
    if (!FPaths::ValidatePath(DataPath, &OutFailureReason))
    {
        return false;
    }

    if (!PlatformFile.CreateDirectoryTree(*DataPath))
    {
        OutFailureReason.FromString(FString::Printf(TEXT("Failed to create directory %s"), *DataPath));
        return false;
    }

    const FString GridDataFilePath = DataPath + GridProxy->GridId.ToString();
    if (!FPaths::ValidatePath(GridDataFilePath, &OutFailureReason))
    {
        return false;
    }

    if (!FPaths::FileExists(GridDataFilePath))
    {
        IFileHandle* FileHandle = PlatformFile.OpenWrite(*GridDataFilePath);
        if (!FileHandle)
        {
            OutFailureReason.FromString(FString::Printf(TEXT("Failed to open platform file handle to %s"), *GridDataFilePath));
            return false;
        }

        //Create an empty file
        if (!FileHandle->Write(nullptr, 0))
        {
            OutFailureReason.FromString(FString::Printf(TEXT("Failed to create %s"), *GridDataFilePath));
            return false;
        }
    }

    GridProxy->DataFilePath = GridDataFilePath;
    const bool GridDataFileExists = FPaths::FileExists(GridDataFilePath);
    if (!GridDataFileExists)
    {
        OutFailureReason.FromString(FString::Printf(TEXT("%s was created but cannot be found (perhaps check permissions)"), *GridDataFilePath));
    }
    return GridDataFileExists;
}

bool UVoxelDatabaseProxy::LoadGridData(AVoxelGridProxy* GridProxy)
{
    return GridProxy && GridProxy->LoadVoxelData();
}

bool UVoxelDatabaseProxy::SaveGridData(AVoxelGridProxy* GridProxy)
{
    FText FailureReason;
    if (!UVoxelDatabaseProxy::CreateGridDataFile(GridProxy, FailureReason))
    {
        //TODO log error (failed to create grid data file for saving)
        FailureReason = FText::FromString(FString::Printf(TEXT("VoxelDatabaseProxy: Failed to create grid data file for saving (%s)"), *FailureReason.ToString()));
        return false;
    }
    return GridProxy->SaveVoxelData();
}

AVoxelGridProxy* UVoxelDatabaseProxy::AddGridProxy(EVoxelDatabaseVoxelType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    FGuid GridId;
    AVoxelGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelDatabaseVoxelType::Bool && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseBoolVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyBool>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Float && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseFloatVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyFloat>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Double && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseDoubleVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyDouble>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt8Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt8>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt16Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt16>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt32Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt32>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Uint64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt64Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt64>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt8Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt8>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt16Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt16>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt32Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt32>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Int64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt64Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt64>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVectorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector4 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector4Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector4>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Vector2D && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector2DVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector2D>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::Color && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseColorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyColor>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::LinearColor && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseLinearColorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyLinearColor>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedNormal && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedNormalVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedNormal>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedRGB10A2N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGB10A2NVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGB10A2N>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::PackedRGBA16N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGBA16NVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGBA16N>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::IntPoint && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntPointVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntPoint>(this, FName(EName::NAME_Actor, *GridDisplayText.ToString()));
    }
    else if (VoxelType == EVoxelDatabaseVoxelType::IntVector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntVectorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
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
    else
    {
        //TODO log error (failed to create grid proxy)
    }
    return GridProxy;
}
