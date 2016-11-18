#pragma once
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseProxy.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>
#include "VoxelDatabase.generated.h"

//TODO Use FText for any strings displayed to the user
//see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/FText/
//DECLARE_MULTICAST_DELEGATE_OneParam(FVoxelDatabaseOnMetadataChanged, class UVoxelDatabaseProxy*, const FGuid&);

UCLASS(ClassGroup = VoxelMate, NotPlaceable, NotBlueprintable, CustomConstructor)
class VOXELMATE_API UVoxelDatabase : public UObject
{
    GENERATED_BODY()

public:
    static UVoxelDatabase& Get()
    {
        extern UVoxelDatabase* VoxelMateVoxelDatabase;
        check(VoxelMateVoxelDatabase);
        return *VoxelMateVoxelDatabase;
    }

    UPROPERTY(BlueprintReadOnly)
        UVoxelDatabase* VoxelDatabase;
    UPROPERTY()
        UVoxelDatabaseProxy* VoxelDatabaseProxy;
    UPROPERTY()
        FString DataPath;
    
    friend FArchive& operator<<(FArchive& Ar, UVoxelDatabase& VoxelDatabase)
    {
        if (!VoxelDatabase.IsDefaultSubobject())
        {
            Ar << VoxelDatabase.Metadata;
            Ar << VoxelDatabase.Grids; //TODO handle grids that share trees
        }
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Ar << *this;
    }

    virtual void BeginDestroy() override
    {
        Super::BeginDestroy();
    }

    void InitializeTypes();
    void UninitializeTypes();

    const TArray<FString>& GetRegisteredGridTypeNames() const;
    const TArray<FString>& GetRegisteredMetadataTypeNames() const;
    const TArray<FString>& GetRegisteredTransformMapTypeNames() const;

    void GetGridsDisplay(TMap<FGuid, FText>& OutGridsDisplay) const;
    void GetMetadataDisplay(TMap<FGuid, FText>& OutMetadataDisplay) const;
    UVoxelDatabaseProxy* GetDatabaseProxy();
    bool AddMetadata(const FString& TypeName, FGuid& OutMetadataId);
    //TODO bool AddGridInstance

    template<typename VoxelType>
    bool UVoxelDatabase::AddGrid(const FText& GridDisplayText, bool SaveFloatAsHalf, FGuid& OutGridId)
    {
        bool IsGridAdded = false;
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create<VoxelType>();
        if (GridPtr != nullptr)
        {
            OutGridId = FGuid::NewGuid();
            check(OutGridId.IsValid());
            GridPtr->setName(TCHAR_TO_UTF8(*OutGridId.ToString()));
            GridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
            GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText), openvdb::TypedMetadata<FVoxelDatabaseTextMeta>(GridDisplayText));
            Grids.Add(OutGridId, GridPtr);
            IsGridAdded = true;
        }

        if(!IsGridAdded)
        {
            OutGridId.Invalidate();
        }
        return IsGridAdded;
    }

    template<typename VoxelType>
    bool UVoxelDatabase::ShallowCopyGrid(const FGuid& InGridId, const FText& GridDisplayText, FGuid& OutGridId)
    {
        bool IsGridCopied = false;
        if (InGridId.IsValid())
        {
            FGridFactory::ValueTypePtr* FindGrid = Grids.Find(InGridId);
            if (FindGrid)
            {
                FGridFactory::ValueTypeConstPtr& OriginalGridPtr = *FindGrid;
                if (OriginalGridPtr != nullptr)
                {
                    FGridFactory::ValueTypePtr GridPtr = FGridFactory::ShallowCopyGrid<VoxelType>(OriginalGridPtr);
                    if (GridPtr != nullptr)
                    {
                        GridPtr->removeMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText));
                        GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText), openvdb::TypedMetadata<FVoxelDatabaseTextMeta>(GridDisplayText));
                        OutGridId = FGuid::NewGuid();
                        check(OutGridId.IsValid());
                        Grids.Add(OutGridId, GridPtr);
                        IsGridCopied = true;
                    }
                }
                else
                {
                    //TODO log error (tried to copy an invalid grid)
                }
            }
            else
            {
                //TODO log error (could not find grid to copy)
            }
        }
        
        if (!IsGridAdded)
        {
            OutGridId.Invalidate();
        }
        return IsGridCopied;
    }

    template<typename VoxelType>
    bool UVoxelDatabase::DeepCopyGrid(const FGuid& InGridId, const FText& GridDisplayText, FGuid& OutGridId)
    {
        bool IsGridCopied = false;
        if (InGridId.IsValid())
        {
            FGridFactory::ValueTypePtr* FindGrid = Grids.Find(InGridId);
            if (FindGrid)
            {
                FGridFactory::ValueTypeConstPtr& OriginalGridPtr = *FindGrid;
                if (OriginalGridPtr != nullptr)
                {
                    FGridFactory::ValueTypePtr GridPtr = FGridFactory::DeepCopyGrid<VoxelType>(OriginalGridPtr);
                    if (GridPtr != nullptr)
                    {
                        GridPtr->removeMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText));
                        GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText), openvdb::TypedMetadata<FVoxelDatabaseTextMeta>(GridDisplayText));
                        OutGridId = FGuid::NewGuid();
                        check(OutGridId.IsValid());
                        Grids.Add(OutGridId, GridPtr);
                        IsGridCopied = true;
                    }
                }
                else
                {
                    //TODO log error (tried to copy an invalid grid)
                }
            }
            else
            {
                //TODO log error (could not find grid to copy)
            }
        }

        if (!IsGridAdded)
        {
            OutGridId.Invalidate();
        }
        return IsGridCopied;
    }

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                return Accessor.getValue(Coord);
            }
        }

        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE const VoxelType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, bool& OutIsVoxelActive) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                OutIsVoxelActive = Accessor.isValueOn(Coord);
                return Accessor.getValue(Coord);
            }
        }

        OutIsVoxelActive = false;
        const static VoxelType Zero = VoxelType::ZeroValue;
        return Zero;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void GetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord, bool& OutIsActive) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                OutIsActive = Accessor.isValueOn(Coord);
            }
        }
        OutIsActive = false;
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                Accessor.setValueOnly(Coord, InVoxelValue);
            }
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const VoxelType& InVoxelValue, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);

                if (InIsActive)
                {
                    Accessor.setValueOn(Coord, InVoxelValue);
                }
                else
                {
                    Accessor.setValueOff(Coord, InVoxelValue);
                }
            }
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void SetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                Accessor.setActiveState(Coord, InIsActive);
            }
        }
    }

    template<typename MetadataType>
    bool UVoxelDatabase::AddMetadata(FGuid& OutMetadataId)
    {
        bool IsMetadataAdded = false;
        FMetaValueFactory::ValueTypePtr MetaPtr = FMetaValueFactory::Create<MetadataType>();
        if (MetaPtr != nullptr)
        {
            OutMetadataId = FGuid::NewGuid();
            check(!Metadata.Contains(OutMetadataId));
            Metadata.Add(OutMetadataId, MetaPtr);
            IsMetadataAdded = true;
        }
        return IsMetadataAdded;
    }


private:
    friend class FVoxelMateModule;
    //TODO idea for faster grid lookup
    //friend class AVoxelGridProxy;

    UVoxelDatabase()
    {
        check(!VoxelDatabase);
        VoxelDatabase = this;
        AddToRoot();

        FString VoxelDatabaseName;
        GetName(VoxelDatabaseName);
        DataPath = FPaths::GameUserDeveloperDir() + VoxelDatabaseName + TEXT("/Data/");
    }

    TMap<FGuid, FGridFactory::ValueTypePtr> Grids;
    TMap<FGuid, FMetaValueFactory::ValueTypePtr> Metadata;
};
