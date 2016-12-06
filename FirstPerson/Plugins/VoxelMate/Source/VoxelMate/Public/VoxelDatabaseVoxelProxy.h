#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseVoxelProxy.generated.h"

//Among the following, these data types are not supported by blueprints and thus the struct is not blueprintable:
// double, int8, int16, int64, uint16, uint32, uint64
USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelBool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        bool Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelDouble
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        double Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelFloat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        float Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelInt8
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        int8 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelInt16
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        int16 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelInt32
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        int32 Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelInt64
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        int64 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelUInt8
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        uint8 Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelUInt16
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        uint16 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelUInt32
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        uint32 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT()
struct VOXELMATE_API FVoxelUInt64
{
    GENERATED_BODY()

    UPROPERTY()
        FIntVector IndexCoord;
    UPROPERTY()
        uint64 Value;
    UPROPERTY()
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelVector
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FVector Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelVector4
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FVector4 Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelVector2D
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FVector2D Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelColor
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FColor Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelLinearColor
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FLinearColor Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelPackedNormal
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FPackedNormal Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelPackedRGB10A2N
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FPackedRGB10A2N Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelPackedRGBA16N
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FPackedRGBA16N Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelIntPoint
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FIntPoint Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

USTRUCT(BlueprintType)
struct VOXELMATE_API FVoxelIntVector
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
        FIntVector IndexCoord;
    UPROPERTY(BlueprintReadWrite)
        FIntVector Value;
    UPROPERTY(BlueprintReadWrite)
        bool IsActive;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyBool : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyBool(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelBool VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyDouble : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyDouble(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelDouble VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyFloat : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyFloat(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelFloat VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyInt8 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelInt8 VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyInt16 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelInt16 VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyInt32 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyInt32(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelInt32 VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyInt64 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelInt64 VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyUInt8 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyUInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelUInt8 VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyUInt16 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyUInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelUInt16 VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyUInt32 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyUInt32(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelUInt32 VoxelData;
};

UCLASS(ClassGroup = VoxelMate)
class VOXELMATE_API AVoxelProxyUInt64 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyUInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY()
        FVoxelUInt64 VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyVector : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyVector(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelVector VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyVector4 : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyVector4(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelVector4 VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyVector2D : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyVector2D(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelVector2D VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyColor : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyColor(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelColor VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyLinearColor : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyLinearColor(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelLinearColor VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyPackedNormal : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyPackedNormal(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelPackedNormal VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyPackedRGB10A2N : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyPackedRGB10A2N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelPackedRGB10A2N VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyPackedRGBA16N : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyPackedRGBA16N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelPackedRGBA16N VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyIntPoint : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyIntPoint(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelIntPoint VoxelData;
};

UCLASS(ClassGroup = VoxelMate, Blueprintable)
class VOXELMATE_API AVoxelProxyIntVector : public AActor
{
    GENERATED_BODY()

public:
    AVoxelProxyIntVector(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {}

    UPROPERTY(BlueprintReadWrite)
        FVoxelIntVector VoxelData;
};
