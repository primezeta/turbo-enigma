#pragma once
#include "EngineMinimal.h"
#include "GridRenderResource.h"
#include "VoxelData.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "GridSpecifier.h"
#include "MetadataSpecifier.h"
#include "VoxelDatabase.generated.h"

UCLASS(ClassGroup=VoxelMate, BlueprintType, Blueprintable, Config=Editor)
class VOXELMATE_API UVoxelDatabase : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(Config)
        FString DatabaseName;
    UPROPERTY(Config)
        bool IsGridInstancingEnabled;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FGridSpecifier> Grids;

    UVoxelDatabase(const FObjectInitializer& ObjectInitializer)
	{
        DatabaseName = TEXT("Voxel Database");
        IsGridInstancingEnabled = true;
	}

    void Init(FGridRenderResource* InResource, bool bInstanced)
    {
        //TODO
    }

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, UVoxelDatabase& VoxelDatabase)
    {
        Ar << VoxelDatabase.DatabaseHeader;
        Ar << VoxelDatabase.DatabaseMetadata;
        Ar << VoxelDatabase.GridData;
        return Ar;
    }

    void GetGridNames(TArray<FString> &OutGridNames) const
    {
        for (auto i = GridData.CreateConstIterator(); i; ++i)
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

    bool AddGrid(const FGridSpecifier& GridSpecifier)
    {
        bool IsGridAdded = false;
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(GridSpecifier.Type);
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create(TypeName);
        if (GridPtr != nullptr)
        {
            check(!Grids.ContainsByPredicate([&](const FGridSpecifier& gs) { return GridSpecifier.Name == gs.Name; })); //TODO handle name clashes
            GridPtr->setName(TCHAR_TO_UTF8(*GridSpecifier.Name));
            GridPtr->setSaveFloatAsHalf(GridSpecifier.SaveFloatAsHalf);
            GridData[GridSpecifier.Name] = GridPtr;
            Grids.Add(GridSpecifier);
            IsGridAdded = true;
        }
        return IsGridAdded;
    }

    //TODO
    //bool AddGridInstance

    bool AddMetadata(const FMetadataSpecifier& MetadataSpecifier)
    {
        bool IsMetadataAdded = false;
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(MetadataSpecifier.Type);
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create(TypeName);
        if (MetaPtr != nullptr)
        {
            check(!DatabaseMetadata.Contains(MetadataSpecifier.Name)); //TODO handle name clashes
            DatabaseMetadata[MetadataSpecifier.Name] = MetaPtr;
            IsMetadataAdded = true;
        }
        return IsMetadataAdded;
    }

    //UObject interface
    virtual void Serialize(FArchive& Ar) override
    {
        Ar << *this;
        Super::Serialize(Ar);
    }

    virtual void BeginDestroy() override
    {
        Super::BeginDestroy();
    }

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
    {
        //if (PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Add       ||
        //    PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Duplicate ||
        //    PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Insert)
        //{
        //    const int32& Index = PropertyChangedEvent.GetArrayIndex();
        //    const FString& GridName = Grids[Index];
        //    AddGrid()
        //}
        //else if (PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Clear ||
        //         PropertyChangedEvent.ChangeType == EPropertyArrayChangeType::Delete)
        //{

        //}
        Super::PostEditChangeProperty(PropertyChangedEvent);
    }
#endif // WITH_EDITOR

    virtual void PostInitProperties() override
    {
        Super::PostInitProperties();
    }

    virtual void PreSave(const class ITargetPlatform* TargetPlatform) override
    {
        Super::PreSave(TargetPlatform);
    }

    virtual void PostLoad() override
    {
        Super::PostLoad();
    }

protected:
    FGridRenderResource * RenderResource;
    FVoxelDatabaseHeader DatabaseHeader;
    TMap<FString, FMetaValueFactory::ValueTypePtr> DatabaseMetadata;
    TMap<FString, FGridFactory::ValueTypePtr> GridData;
};
