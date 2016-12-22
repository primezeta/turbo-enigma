#pragma once
#include "VoxelDatabaseCommon.h"
#include "VoxelGridProxy.h"
#include "VoxelDatabaseProxy.generated.h"

UCLASS(ClassGroup = VoxelMate, NotPlaceable, BlueprintType, CustomConstructor)
class VOXELMATE_API AVoxelDatabaseProxy : public AActor
{
    GENERATED_BODY()

public:
    friend FArchive& operator<<(FArchive& Ar, AVoxelDatabaseProxy& DatabaseProxy)
    {
        if (!DatabaseProxy.IsDefaultSubobject())
        {
            Ar << DatabaseProxy.GridProxies;
        }
        return Ar;
    }

    virtual void AVoxelDatabaseProxy::Serialize(FArchive& Ar) override
    {
        Super::Serialize(Ar);
        Ar << *this;
    }

    virtual bool IsReadyForFinishDestroy() override;
    virtual void BeginDestroy() override;

    UPROPERTY(Category = VoxelDatabaseProxy, BlueprintReadOnly, Replicated)
        TArray<AVoxelGridProxy*> GridProxies;
    //UPROPERTY()
        //UVoxelMetadataProxy* DatabaseMetadataProxy; //TODO

	UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
		static AVoxelDatabaseProxy* InitVoxelDatabase();
	UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
		void LoadVoxelDatabase(const FString& DatabaseDirectory);
	UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
		void SaveVoxelDatabase(const FString& DatabaseDirectory);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddScalarGrid(EVoxelScalarType VoxelType, const FText& GridDisplayText);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddFloatScalarGrid(EVoxelFloatScalarType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddVectorGrid(EVoxelVectorType VoxelType, const FText& GridDisplayText);
    UFUNCTION(Category = VoxelDatabaseProxy, BlueprintCallable)
        AVoxelGridProxy* AddFloatVectorGrid(EVoxelFloatVectorType VoxelType, const FText& GridDisplayText, bool SaveFloatAsHalf);

private:
    AVoxelDatabaseProxy();
};
