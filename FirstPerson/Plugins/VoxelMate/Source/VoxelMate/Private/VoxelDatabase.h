#pragma once
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"

//TODO Use FText for any strings displayed to the user
//see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/FText/

struct VOXELMATE_API FVoxelDatabase
{
    FString DatabaseName;
    bool IsGridInstancingEnabled;

    FVoxelDatabase()
    {}

    FVoxelDatabase(const FString& Name, bool EnableGridInstancing)
        : DatabaseName(Name), IsGridInstancingEnabled(EnableGridInstancing)
    {}

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoxelDatabase& VoxelDatabase);
    static void InitializeTypes();
    static void UninitializeTypes();

    const TArray<FString>& GetRegisteredGridTypeNames() const;
    const TArray<FString>& GetRegisteredMetadataTypeNames() const;
    const TArray<FString>& GetRegisteredTransformMapTypeNames() const;

    void GetGridsDisplay(TMap<FGuid, FText>& OutGridsDisplay) const;
    void GetMetadataDisplay(TMap<FGuid, FText>& OutMetadataDisplay) const;
    bool AddGrid(const FString& TypeName, const FText& GridDisplayText, bool SaveFloatAsHalf, FGuid& OutGridId);
    bool AddMetadata(const FString& TypeName, FGuid& OutMetadataId);
    //TODO bool AddGridInstance

private:
    TMap<FGuid, FMetaValueFactory::ValueTypePtr> Metadata;
    TMap<FGuid, FGridFactory::ValueTypePtr> Grids;
};
