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
    AVoxelGridProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
    {
        GridMeshComponent = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("GridMeshComponent"));
        GridDisplayText = FText::FromString(GetName());
        DataFilePath = TEXT("");
        //TODO idea for faster grid lookup
        //UVoxelDatabase::Get().Grids
        //CacheIndex = 
    }

    friend FArchive& operator<<(FArchive& Ar, AVoxelGridProxy& GridProxy)
    {
        if (!GridProxy.IsDefaultSubobject())
        {
            Ar << GridProxy.GridId;
            Ar << GridProxy.GridDisplayText;
            Ar << GridProxy.DataFilePath;
        }
        return Ar;
    }
    
    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
        Ar << *this;
    }

    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
        FGuid GridId;
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
        FText GridDisplayText;
    UPROPERTY(Category = VoxelGridProxy, BlueprintReadOnly)
        FString DataFilePath;
    //UPROPERTY()
    //    TArray<UVoxelMetadataProxy*> MetadataProxies; //TODO
    UPROPERTY()
        UProceduralMeshComponent* GridMeshComponent;

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAScalarType()
        {
            return false;
        }
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAVectorType()
        {
            return false;
        }
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAFloatType()
        {
            return false;
        }

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        bool LoadVoxelData();
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        bool SaveVoxelData();

protected:
    //TODO idea for faster grid lookup
    //int32 CacheIndex;
};

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable)
class VOXELMATE_API AVoxelScalarGridProxy : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAScalarType() override final
        {
            return true;
        }
};

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable)
class VOXELMATE_API AVoxelVectorGridProxy : public AVoxelGridProxy
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAVectorType() override final
        {
            return true;
        }
};

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable)
class VOXELMATE_API AVoxelFloatScalarGridProxy : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
    AVoxelFloatScalarGridProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer), IsFloatSavedAsHalf(false)
    {}

    friend FArchive& operator<<(FArchive& Ar, AVoxelFloatScalarGridProxy& GridProxy)
    {
        if (!GridProxy.IsDefaultSubobject())
        {
            Ar << GridProxy.IsFloatSavedAsHalf;
        }
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
        Ar << *this;
    }

    UPROPERTY(BlueprintReadOnly)
        bool IsFloatSavedAsHalf;

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAFloatType() override final
        {
            return true;
        }
};

UCLASS(ClassGroup = VoxelMate, Abstract, NotPlaceable)
class VOXELMATE_API AVoxelFloatVectorGridProxy : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
    AVoxelFloatVectorGridProxy(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer), IsFloatSavedAsHalf(false)
    {}

    friend FArchive& operator<<(FArchive& Ar, AVoxelFloatVectorGridProxy& GridProxy)
    {
        if (!GridProxy.IsDefaultSubobject())
        {
            Ar << GridProxy.IsFloatSavedAsHalf;
        }
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
        Ar << *this;
    }

    UPROPERTY(BlueprintReadOnly)
        bool IsFloatSavedAsHalf;

    UFUNCTION(Category = VoxelGridProxy, BlueprintCallable)
        virtual bool IsAFloatType() override final
        {
            return true;
        }
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType)
class VOXELMATE_API AVoxelGridProxyBool : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyColor : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyDouble : public AVoxelFloatScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyFloat : public AVoxelFloatScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyInt8 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyInt16 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyInt32 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyInt64 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyUInt8 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyUInt16 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyUInt32 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyUInt64 : public AVoxelScalarGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyVector : public AVoxelFloatVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyVector2D : public AVoxelFloatVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyVector4 : public AVoxelFloatVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyIntPoint : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyIntVector : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyLinearColor : public AVoxelFloatVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyPackedNormal : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyPackedRGB10A2N : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
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
class VOXELMATE_API AVoxelGridProxyPackedRGBA16N : public AVoxelVectorGridProxy
{
    GENERATED_BODY()

public:
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
