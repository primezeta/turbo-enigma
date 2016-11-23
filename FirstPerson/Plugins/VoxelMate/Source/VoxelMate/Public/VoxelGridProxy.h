#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseCommon.h"
//#include "Runtime/Engine/Classes/Interfaces/Interface_CollisionDataProvider.h"
//#include "Editor/UnrealEd/Classes/ThumbnailRendering/SceneThumbnailInfo.h"
//#include "PhysicsPublic.h"
#include "ProceduralMeshComponent.h"
#include "BulkData.h"
#include "VoxelGridProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable, Blueprintable)
class VOXELMATE_API AVoxelGridProxy : public AActor
{
    GENERATED_BODY()

public:
    AVoxelGridProxy(const FObjectInitializer& ObjectInitializer);

    friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy);
    virtual void Serialize(FArchive& Ar) override;

    UPROPERTY(BlueprintReadOnly)
        FGuid GridId;
    UPROPERTY(BlueprintReadOnly)
        bool IsFloatSavedAsHalf;
    UPROPERTY(BlueprintReadOnly)
        FText GridDisplayText;
    UPROPERTY(BlueprintReadOnly)
        FString DataFilePath;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY()
        UProceduralMeshComponent* GridMeshComponent;

    UFUNCTION()
        bool LoadVoxelData();
    UFUNCTION()
        bool SaveVoxelData();

protected:
    //TODO idea for faster grid lookup
    //int32 CacheIndex;
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyBool : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyBool(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const bool& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const bool& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const bool& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const bool& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyColor : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyColor(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FColor& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FColor& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FColor& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FColor& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FColor& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable)
class VOXELMATE_API AVoxelGridProxyDouble : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyDouble(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //double is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const double& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const double& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const double& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const double& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const double& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyFloat(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const float& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const float& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const float& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const float& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const float& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int8 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const int8& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const int8& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const int8& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int8& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int8& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt16 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int16 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const int16& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const int16& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const int16& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int16& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int16& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt32 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt32(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const int32& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const int32& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const int32& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int32& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int32& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //int64 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const int64& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const int64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const int64& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const int64& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const int64& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt8(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const uint8& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const uint8& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint8& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint8& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint8& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt16 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt16(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint16 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const uint16& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const uint16& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint16& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint16& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint16& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt32 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt32(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint32 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const uint32& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const uint32& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint32& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint32& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint32& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyUInt64 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyUInt64(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    //uint32 is not a supported blueprint type
    UFUNCTION(Category = VoxelGridProxy)
        const uint64& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        const uint64& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValue(const FIntVector& IndexCoord, const uint64& InValue);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const uint64& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const uint64& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FVector& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector2D : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector2D(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector2D& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector2D& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector2D& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector2D& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FVector2D& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyVector4 : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyVector4(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector4& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FVector4& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FVector4& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FVector4& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FVector4& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyIntPoint : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyIntPoint(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntPoint& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntPoint& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FIntPoint& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FIntPoint& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FIntPoint& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyIntVector : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyIntVector(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntVector& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FIntVector& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FIntVector& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FIntVector& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FIntVector& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyLinearColor : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyLinearColor(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FLinearColor& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FLinearColor& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FLinearColor& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FLinearColor& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FLinearColor& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedNormal : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedNormal(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedNormal& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedNormal& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedNormal& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedNormal& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FPackedNormal& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedRGB10A2N : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedRGB10A2N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGB10A2N& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGB10A2N& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedRGB10A2N& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedRGB10A2N& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FPackedRGB10A2N& InValue, const bool& InIsActive);
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyPackedRGBA16N : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    AVoxelGridProxyPackedRGBA16N(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
    }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGBA16N& GetVoxelValue(const FIntVector& IndexCoord) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void GetVoxelIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        const FPackedRGBA16N& GetVoxelValueAndIsActive(const FIntVector& IndexCoord, bool& OutIsActive) const;
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValue(const FIntVector& IndexCoord, const FPackedRGBA16N& InValue);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelIsActive(const FIntVector& IndexCoord, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void SetVoxelValueAndIsActive(const FIntVector& IndexCoord, const FPackedRGBA16N& InValue, const bool& InIsActive);
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        void Fill(const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const FPackedRGBA16N& InValue, const bool& InIsActive);
};
