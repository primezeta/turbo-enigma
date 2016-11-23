#pragma once
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseProxy.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>
#include <openvdb/tools/ValueTransformer.h>
#include "VoxelDatabase.generated.h"

//TODO Use FText for any strings displayed to the user
//see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/FText/
//DECLARE_MULTICAST_DELEGATE_OneParam(FVoxelDatabaseOnMetadataChanged, class UVoxelDatabaseProxy*, const FGuid&);

template<typename IterT, typename VoxelT>
struct NoiseValueOp
{
    typedef typename VoxelT VoxelType;
    typedef typename VoxelT::ValueType ValueType;
    typedef typename IterT IterType;

    const TArray<TArray<TArrayView<float>>>& Noise3DValues;
    NoiseValueOp(const TArray<TArray<TArrayView<float>>>& Values) : Noise3DValues(Values) {}
    VOXELMATEINLINE void operator()(const IterType& iter) const
    {
        check(iter.isVoxelValue());
        const openvdb::Coord& coord = iter.getCoord();
        iter.setValue(Noise3DValues[coord.x()][coord.y()][coord.z()]);
    }
};

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
        OutGridId = FGuid::NewGuid();
        if (OutGridId.IsValid())
        {
            FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create<VoxelType>();
            GridPtr->setName(TCHAR_TO_UTF8(*OutGridId.ToString()));
            GridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
            FMetaMapFactory::InsertGridMeta<FText>(*GridPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, GridDisplayText);
            Grids.Add(OutGridId, GridPtr);
            IsGridAdded = true;
        }
        else
        {
            //TODO log warning (new grid id not valid)
            OutGridId.Invalidate(); //set all to 0
        }
        return IsGridAdded;
    }

    template<typename VoxelType>
    int32 UVoxelDatabase::ShallowCopyGrid(const FGuid& InGridId, const FText& GridDisplayText)
    {
        int32 GridCopyIndex = -1;
        FGridFactory::ValueTypePtr GridCopyPtr = FGridFactory::ShallowCopyGrid<VoxelType>(OriginalGridPtr);
        if (GridCopyPtr != nullptr)
        {
            FMetaMapFactory::InsertGridMeta<FText>(*GridCopyPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, GridDisplayText);

            const openvdb::Name& OriginalGridName = GridPtr->getName();
            GridCopyIndex = GridCopies.Num(InGridId);

            std::ostringstream GridCopyName;
            GridCopyName << GridPtr->getName() << "_" << GridCopyIndex;
            GridPtr->setName(GridCopyName.str());

            GridCopies.AddUnique(InGridId, GridCopyPtr);
            IsGridCopied = true;
        }
        else
        {
            //TODO log error (could not copy grid; possibly different types)
        }
        return GridCopyIndex;
    }

    template<typename VoxelType>
    int32 UVoxelDatabase::DeepCopyGrid(const FGuid& InGridId, const FText& GridDisplayText)
    {
        int32 GridCopyIndex = -1;
        FGridFactory::ValueTypePtr GridCopyPtr = FGridFactory::DeepCopyGrid<VoxelType>(OriginalGridPtr);
        if (GridCopyPtr != nullptr)
        {
            FMetaMapFactory::InsertGridMeta<FText>(*GridCopyPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, GridDisplayText);

            const openvdb::Name& OriginalGridName = GridPtr->getName();
            GridCopyIndex = GridCopies.Num(InGridId);

            std::ostringstream GridCopyName;
            GridCopyName << GridPtr->getName() << "_" << GridCopyIndex;
            GridPtr->setName(GridCopyName.str());

            GridCopies.AddUnique(InGridId, GridCopyPtr);
            IsGridCopied = true;
        }
        else
        {
            //TODO log error (could not copy grid; possibly different types)
        }
        return GridCopyIndex;
    }

    template<typename VoxelType>
    bool UVoxelDatabase::DuplicateGrid(const FGuid& InGridId, const FText& GridDisplayText, FGuid& OutDuplicateGridId)
    {
        FGridFactory::ValueTypePtr* FindGrid = Grids.Find(InGridId);
        if (!FindGrid)
        {
            //TODO log error (grid already exists - cannot overwrite it with the duplicate)
            return false;
        }

        bool IsGridDuplicated = false;
        FGridFactory::ValueTypePtr GridDuplicatePtr = FGridFactory::DeepCopyGrid<VoxelType>(*FindGrid);
        if (GridDuplicatePtr != nullptr)
        {
            OutDuplicateGridId = FGuid::NewGuid();
            check(!Grids.Contains(OutDuplicateGridId));
            GridPtr->setName(TCHAR_TO_UTF8(*OutDuplicateGridId.ToString()));
            //Note:: save-float-as-half is automatically copied via the grid metadata
            FMetaMapFactory::InsertGridMeta<FText>(*GridPtr, VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText, GridDisplayText);
            Grids.Add(OutDuplicateGridId, GridDuplicatePtr);
            IsGridDuplicated = true;
        }
        else
        {
            //TODO log error (could not copy grid; possibly different types)
        }
        return IsGridDuplicated;
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void FillGrid(const FGuid& GridId, const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const VoxelType& InValue, const bool& InIsActive)
    {
        if (FillDimensions.X == 0 || FillDimensions.Y == 0 || FillDimensions.Z == 0)
        {
            //TODO log error (fill volume is empty)
            return;
        }

        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                //const CoordBBox& bbox, const ValueType& value, bool active = true
                const openvdb::Coord FillStart(StartIndexCoord.X, StartIndexCoord.Y, StartIndexCoord.Z);
                const openvdb::Coord FillEnd(FillStart.x() + FillDimensions.X, FillStart.y() + FillDimensions.Y, FillStart.z() + FillDimensions.Z);
                const openvdb::CoordBBox FillBBox(FillStart, FillEnd);
                TypedGridPtr->fill(FillBBox, InValue, InIsActive);
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    template<typename ValueOpType>
    VOXELMATEINLINE void RunGridOp(const FGuid& GridId, ValueOpType& ValueOp)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueOpType::VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueOpType::VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::tools::foreach<ValueOpType::IterType, ValueOpType>(TypedGridPtr->beginValueOn(), ValueOp);
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
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
        AddToRoot();
        FString VoxelDatabaseName;
        GetName(VoxelDatabaseName);
        DataPath = FPaths::GameUserDeveloperDir() + VoxelDatabaseName + TEXT("/Data/");
    }

    TMap<FGuid, FMetaValueFactory::ValueTypePtr> Metadata;
    TMap<FGuid, FGridFactory::ValueTypePtr> Grids;
    TMultiMap<FGuid, FGridFactory::ValueTypePtr> GridCopies;
};
