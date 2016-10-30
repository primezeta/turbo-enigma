#pragma once
#include "EngineMinimal.h"
#include "GridRenderResource.h"
#include "VoxelData.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabaseGridTypeSpecifier.h"
#include "VoxelDatabaseMetadataTypeSpecifier.h"
#include "VoxelDatabaseTransformMapTypeSpecifier.h"
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite) //TODO investigate making Grids NonTransactional
        TArray<FVoxelDatabaseGridTypeSpecifier> Grids;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FVoxelDatabaseMetadataTypeSpecifier> Metadata;

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
            if (i->Value != nullptr)
            {
                FString GridName;
                const openvdb::Name MetaNameGridName = TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayName);
                auto MetaValuePtr = i->Value->getMetadata<openvdb::TypedMetadata<FString>>(MetaNameGridName);
                check(MetaValuePtr != nullptr);
                OutGridNames.Add(MetaValuePtr->value());
            }
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

    bool AddGrid(const FVoxelDatabaseGridTypeSpecifier& GridTypeSpecifier, FString& OutGridId)
    {
        bool IsGridAdded = false;
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create(GridTypeSpecifier);
        if (GridPtr != nullptr)
        {
            TFunctionRef<bool(const FVoxelDatabaseGridTypeSpecifier&)> FindGridByName = [&](const FVoxelDatabaseGridTypeSpecifier& gs) { return GridTypeSpecifier.Name == gs.Name; };
            FVoxelDatabaseGridTypeSpecifier* Specifier = Grids.FindByPredicate(FindGridByName);
            if (Specifier)
            {
                Specifier->NameCount++;
            }
            else
            {
                Grids.Add(GridTypeSpecifier);
            }

            const FGuid UniqueId = FGuid::NewGuid();
            OutGridId = UniqueId.ToString();
            GridPtr->setName(TCHAR_TO_UTF8(*OutGridId));
            GridPtr->setSaveFloatAsHalf(GridTypeSpecifier.SaveFloatAsHalf);
            GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayName), openvdb::TypedMetadata<FString>(GridTypeSpecifier.Name));

            check(!GridData.Contains(OutGridId));
            GridData.Add(OutGridId, GridPtr);
            IsGridAdded = true;
        }
        return IsGridAdded;
    }

    //TODO
    //bool AddGridInstance

    bool AddMetadata(const FVoxelDatabaseMetadataTypeSpecifier& MetadataTypeSpecifier, FString& OutMetadataId)
    {
        bool IsMetadataAdded = false;
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create(MetadataTypeSpecifier);
        if (MetaPtr != nullptr)
        {
            TFunctionRef<bool(const FVoxelDatabaseMetadataTypeSpecifier&)> FindMetadataByName = [&](const FVoxelDatabaseMetadataTypeSpecifier& ms) { return MetadataTypeSpecifier.Name == ms.Name; };
            FVoxelDatabaseMetadataTypeSpecifier* Specifier = Metadata.FindByPredicate(FindMetadataByName);
            if (Specifier)
            {
                Specifier->NameCount++;
            }
            else
            {
                Metadata.Add(MetadataTypeSpecifier);
            }

            const FGuid UniqueId = FGuid::NewGuid();
            OutMetadataId = UniqueId.ToString();

            check(!DatabaseMetadata.Contains(OutMetadataId));
            DatabaseMetadata.Add(OutMetadataId, MetaPtr);
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
