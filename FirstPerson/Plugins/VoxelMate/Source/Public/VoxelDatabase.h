#pragma once
#include "EngineMinimal.h"
#include "VoxelData.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabase.generated.h"

UCLASS(ClassGroup=VoxelMate, Blueprintable, Config=Editor, NotPlaceable)
class VOXELMATE_API UVoxelDatabase : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(Config)
        FString DatabaseName;
    UPROPERTY(Config)
        bool IsGridInstancingEnabled;

    UVoxelDatabase(const FObjectInitializer& ObjectInitializer)
	{
        DatabaseName = TEXT("Voxel Database");
        IsGridInstancingEnabled = true;
	}

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, UVoxelDatabase& VoxelDatabase)
    {
        Ar << VoxelDatabase.DatabaseHeader;
        Ar << VoxelDatabase.DatabaseMetadata;
        Ar << VoxelDatabase.Grids;
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Ar << *this;
        Super::Serialize(Ar);
    }

    void GetGridNames(TArray<FString> &OutGridNames) const
    {
        for (auto i = Grids.CreateConstIterator(); i; ++i)
        {
            OutGridNames.Add(i.Key());
        }
    }

    const TArray<FString>& GetRegisteredGridTypeNames() const
    {
        return FGridFactory::RegisteredTypeNames;
    }

    const TArray<FString>& GetRegisteredMetadataTypeNames() const
    {
        return FMetaValueFactory::RegisteredTypeNames;
    }

    const TArray<FString>& GetRegisteredTransformMapTypeNames() const
    {
        return FTransformMapFactory::RegisteredTypeNames;
    }

    void InitializeGridTypes();

    bool AddGrid(const FString& TypeName, const FString& GridName, bool IsFloatHalf)
    {
        bool IsGridAdded = false;
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create(TCHAR_TO_UTF8(*TypeName));
        if (GridPtr != nullptr)
        {
            GridPtr->setName(TCHAR_TO_UTF8(*GridName));
            GridPtr->setSaveFloatAsHalf(IsFloatHalf);
            Grids[GridName] = GridPtr; //TODO handle name clashes
            IsGridAdded = true;
        }
        return IsGridAdded;
    }

    //TODO
    //bool AddGridInstance

    bool AddMeta(const FString& TypeName, const FString& MetaName)
    {
        bool IsMetaAdded = false;
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create(TCHAR_TO_UTF8(*TypeName));
        if (MetaPtr != nullptr)
        {
            DatabaseMetadata[MetaName] = MetaPtr; //TODO handle name clashes
            IsMetaAdded = true;
        }
        return IsMetaAdded;
    }

protected:
    FVoxelDatabaseHeader DatabaseHeader;
    TMap<FString, FMetaValueFactory::ValueTypePtr> DatabaseMetadata;
    TMap<FString, FGridFactory::ValueTypePtr> Grids;
};
