#include "VoxelMatePCH.h"
//#include "VoxelDatabaseCommonPrivate.h"
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
    UVoxelDatabaseProxy* VoxelDatabaseProxy = UVoxelDatabase::Get().GetDatabaseProxy();
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

AVoxelGridProxy* UVoxelDatabaseProxy::AddScalarGrid(EVoxelScalarType VoxelType, const FText& GridDisplayText)
{
    FGuid GridId;
    AVoxelScalarGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelScalarType::Bool && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseBoolVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyBool>(this);
    }
    else if (VoxelType == EVoxelScalarType::Uint8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt8Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt8>(this);
    }
    else if (VoxelType == EVoxelScalarType::Uint16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt16Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt16>(this);
    }
    else if (VoxelType == EVoxelScalarType::Uint32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt32Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt32>(this);
    }
    else if (VoxelType == EVoxelScalarType::Uint64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseUInt64Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyUInt64>(this);
    }
    else if (VoxelType == EVoxelScalarType::Int8 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt8Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt8>(this);
    }
    else if (VoxelType == EVoxelScalarType::Int16 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt16Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt16>(this);
    }
    else if (VoxelType == EVoxelScalarType::Int32 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt32Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt32>(this);
    }
    else if (VoxelType == EVoxelScalarType::Int64 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseInt64Voxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyInt64>(this);
    }
    else if (VoxelType == EVoxelScalarType::Color && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseColorVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyColor>(this);
    }

    if (GridProxy)
    {
        check(GridId.IsValid());
        GridProxy->GridId = GridId;
        GridProxy->GridDisplayText = GridDisplayText;
        GridProxies.Add(GridProxy);
    }
    else
    {
        //TODO log error (failed to create grid proxy)
    }
    return GridProxy;
}

AVoxelGridProxy* UVoxelDatabaseProxy::AddFloatScalarGrid(EVoxelFloatScalarType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    FGuid GridId;
    AVoxelFloatScalarGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatScalarType::Float && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseFloatVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyFloat>(this);
    }
    else if (VoxelType == EVoxelFloatScalarType::Double && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseDoubleVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyDouble>(this);
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

AVoxelGridProxy* UVoxelDatabaseProxy::AddVectorGrid(EVoxelVectorType VoxelType, const FText& GridDisplayText)
{
    FGuid GridId;
    AVoxelVectorGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelVectorType::PackedNormal && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedNormalVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedNormal>(this);
    }
    else if (VoxelType == EVoxelVectorType::PackedRGB10A2N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGB10A2NVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGB10A2N>(this);
    }
    else if (VoxelType == EVoxelVectorType::PackedRGBA16N && UVoxelDatabase::Get().AddGrid<FVoxelDatabasePackedRGBA16NVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyPackedRGBA16N>(this);
    }
    else if (VoxelType == EVoxelVectorType::IntPoint && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntPointVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntPoint>(this);
    }
    else if (VoxelType == EVoxelVectorType::IntVector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseIntVectorVoxel>(GridDisplayText, false, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyIntVector>(this);
    }

    if (GridProxy)
    {
        check(GridId.IsValid());
        GridProxy->GridId = GridId;
        GridProxy->GridDisplayText = GridDisplayText;
        GridProxies.Add(GridProxy);
    }
    else
    {
        //TODO log error (failed to create grid proxy)
    }
    return GridProxy;
}

AVoxelGridProxy* UVoxelDatabaseProxy::AddFloatVectorGrid(EVoxelFloatVectorType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf)
{
    FGuid GridId;
    AVoxelFloatVectorGridProxy* GridProxy = nullptr;

    if (VoxelType == EVoxelFloatVectorType::Vector && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVectorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector>(this);
    }
    else if (VoxelType == EVoxelFloatVectorType::Vector4 && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector4Voxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector4>(this);
    }
    else if (VoxelType == EVoxelFloatVectorType::Vector2D && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseVector2DVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyVector2D>(this);
    }
    else if (VoxelType == EVoxelFloatVectorType::LinearColor && UVoxelDatabase::Get().AddGrid<FVoxelDatabaseLinearColorVoxel>(GridDisplayText, SaveFloatAsHalf, GridId))
    {
        GridProxy = NewObject<AVoxelGridProxyLinearColor>(this);
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
