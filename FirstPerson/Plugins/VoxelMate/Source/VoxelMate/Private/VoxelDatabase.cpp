#include "VoxelMatePCH.h"
#include "VoxelDatabase.h"
#include "ArchiveTransformMap.h"

AVoxelDatabase* AVoxelDatabase::VoxelMateVoxelDatabase = nullptr;

AVoxelDatabase::AVoxelDatabase()
    : AuthDatabaseProxy(nullptr), LocalDatabaseProxy(nullptr)
{
    SetReplicates(true);
    SetReplicateMovement(false);
    SetActorEnableCollision(false);
    bNetTemporary = 0;
    bAutoDestroyWhenFinished = 1;
    bCanBeDamaged = 0;

    if (GetNetMode() != ENetMode::NM_Client)
    {
        AuthDatabaseProxy = NewObject<AVoxelDatabaseProxy>(this);
		DatabaseProxy = AuthDatabaseProxy;
    }
}

void AVoxelDatabase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVoxelDatabase, DatabaseProxy);
}

void AVoxelDatabase::Startup()
{
    //TODO: Note that the following BLOSC related stuff is in openvdb::initialize
//#ifdef OPENVDB_USE_BLOSC
//    blosc_init();
//    if (blosc_set_compressor("lz4") < 0) {
//        OPENVDB_LOG_WARN("Blosc LZ4 compressor is unavailable");
//    }
//    /// @todo blosc_set_nthreads(int nthreads);
//#endif
    FGridFactory::RegisterSupportedTypes();
    FMetaValueFactory::RegisterSupportedTypes();
    FTransformMapFactory::RegisterSupportedTypes();
}

void AVoxelDatabase::Shutdown()
{
    FGridFactory::UnregisterSupportedTypes();
    FMetaValueFactory::UnregisterSupportedTypes();
    FTransformMapFactory::UnregisterSupportedTypes();
}

const TArray<FString>& AVoxelDatabase::GetRegisteredGridTypeNames() const
{
    return FGridFactory::GetRegisteredTypeNames();
}

const TArray<FString>& AVoxelDatabase::GetRegisteredMetadataTypeNames() const
{
    return FMetaValueFactory::GetRegisteredTypeNames();
}

const TArray<FString>& AVoxelDatabase::GetRegisteredTransformMapTypeNames() const
{
    return FTransformMapFactory::GetRegisteredTypeNames();
}
