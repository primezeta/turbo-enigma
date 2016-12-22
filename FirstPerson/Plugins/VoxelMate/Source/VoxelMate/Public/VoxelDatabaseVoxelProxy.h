#pragma once
#include "EngineMinimal.h"
#include "UnrealNetwork.h"
#include "VoxelDatabaseVoxelProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxy : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxy(const FObjectInitializer &ObjectInitializer)
        : Super(ObjectInitializer), IndexCoord(FIntVector(EForceInit::ForceInitToZero)), IsVoxelActive(false)
    {
		SetReplicates(true);
		//TODO: Maybe have the grid proxy also contain the rendering?
		SetReplicateMovement(false);
		SetActorEnableCollision(false);
		bNetTemporary = 0;
		bAutoDestroyWhenFinished = 1;
		bCanBeDamaged = 0;
    }

    UPROPERTY(BlueprintReadOnly, Transient, Replicated)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        bool IsVoxelActive;
};

#define VOXEL_PROXY_IMPLEMENTATION(VoxelValueType)
//TODO
//#define VOXEL_PROXY_IMPLEMENTATION(VoxelValueType, TypeName)\
//void AVoxelGridProxy##Name::SetVoxel_Implementation(AVoxelProxy* Voxel) const\
//{\
//    AVoxelProxy##Name* VoxelProxy = Cast<AVoxelProxy##Name>(Voxel)\
//    if(VoxelProxy)\
//    {\
//        UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, VoxelProxy->IndexCoord, VoxelProxy->InValue);\
//    }\
//}\
//const ValueType& AVoxelGridProxy##Name::GetVoxelValue(const FIntVector& IndexCoord) const\
//{\
//    return UVoxelDatabase::Get().GetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord).Value;\
//}\
//void AVoxelGridProxy##Name::SetVoxelValue(const FIntVector& IndexCoord, const ValueType& InValue)\
//{\
//    UVoxelDatabase::Get().SetVoxelValue<FVoxelDatabase##Name##Voxel>(GridId, IndexCoord, InValue);\
//}\
//void AVoxelGridProxy##Name::FillValues(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const ValueType& InValue, const bool& InIsActive, const bool& InVoxelizeActiveTilesAfterFill)\
//{\
//    UVoxelDatabase::Get().FillGrid<FVoxelDatabase##Name##Voxel>(GridId, StartIndexCoord, FillDimensions, InValue, InIsActive, InVoxelizeActiveTilesAfterFill);\
//}

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyBool : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(bool)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        bool VoxelValue;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyFloat : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(float)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        float VoxelValue;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyInt32 : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(int32)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        int32 VoxelValue;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyUInt8 : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(uint8)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        uint8 VoxelValue;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyVector : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(FVector)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        FVector VoxelValue;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelProxyIntVector : public AVoxelProxy
{
    GENERATED_BODY()

public:
    VOXEL_PROXY_IMPLEMENTATION(FIntVector)
    UPROPERTY(BlueprintReadWrite, Transient, Replicated)
        FIntVector VoxelValue;
};
