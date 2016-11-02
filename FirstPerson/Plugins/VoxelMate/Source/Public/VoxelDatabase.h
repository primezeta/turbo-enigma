#pragma once
#include "EngineMinimal.h"
#include "GridRenderResource.h"
#include "VoxelData.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "ArchiveTransformMap.h"
#include "VoxelDatabase.generated.h"

//TODO Use FText for any strings displayed to the user
//see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/FText/

USTRUCT(BlueprintType) //TODO Maybe use Atomic = "Indicates that this struct should always be serialized as a single unit."
struct VOXELMATE_API FVoxelDatabase
{
    GENERATED_BODY()

    UPROPERTY()
        FString DatabaseName;
    UPROPERTY()
        bool IsGridInstancingEnabled;

    FVoxelDatabase()
    {}

    FVoxelDatabase(const FString& Name, bool EnableGridInstancing)
        : DatabaseName(Name), IsGridInstancingEnabled(EnableGridInstancing)
    {}

    void Init(FGridRenderResource* InResource, bool bInstanced)
    {
        //TODO
    }

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FVoxelDatabase& VoxelDatabase)
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

    static void InitializeTypes();

    bool AddGrid(const UVoxelDatabaseGridTypeSpecifier& GridTypeSpecifier, FString& OutGridId)
    {
        bool IsGridAdded = false;
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create(GridTypeSpecifier);
        if (GridPtr != nullptr)
        {
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

    bool AddMetadata(const UVoxelDatabaseMetadataTypeSpecifier& MetadataTypeSpecifier, FString& OutMetadataId)
    {
        bool IsMetadataAdded = false;
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create(MetadataTypeSpecifier);
        if (MetaPtr != nullptr)
        {
            const FGuid UniqueId = FGuid::NewGuid();
            OutMetadataId = UniqueId.ToString();

            check(!DatabaseMetadata.Contains(OutMetadataId));
            DatabaseMetadata.Add(OutMetadataId, MetaPtr);
            IsMetadataAdded = true;
        }
        return IsMetadataAdded;
    }

protected:
    FGridRenderResource * RenderResource;
    FVoxelDatabaseHeader DatabaseHeader;
    TMap<FString, FMetaValueFactory::ValueTypePtr> DatabaseMetadata;
    TMap<FString, FGridFactory::ValueTypePtr> GridData;
};
