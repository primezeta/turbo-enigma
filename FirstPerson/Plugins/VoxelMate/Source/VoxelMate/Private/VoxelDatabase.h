#pragma once
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseProxy.h"
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
        bool IsGridInstancingEnabled;
    UPROPERTY()
        UVoxelDatabaseProxy* VoxelDatabaseProxy;
    
    friend FArchive& operator<<(FArchive& Ar, UVoxelDatabase& VoxelDatabase)
    {
        if (!VoxelDatabase.IsDefaultSubobject())
        {
            Ar << VoxelDatabase.IsGridInstancingEnabled;
            Ar << VoxelDatabase.Metadata;
            Ar << VoxelDatabase.Grids;
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

    template<typename ValueType>
    bool UVoxelDatabase::AddGrid(const FText& GridDisplayText, bool SaveFloatAsHalf, FGuid& OutGridId)
    {
        bool IsGridAdded = false;
        FGridFactory::ValueTypePtr GridPtr = FGridFactory::Create(UTF8_TO_TCHAR(openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::gridType().c_str()));
        if (GridPtr != nullptr)
        {
            OutGridId = FGuid::NewGuid();
            check(!Grids.Contains(OutGridId));
            GridPtr->setName(TCHAR_TO_UTF8(*OutGridId.ToString()));
            GridPtr->setSaveFloatAsHalf(SaveFloatAsHalf);
            GridPtr->insertMeta(TCHAR_TO_UTF8(*VoxelDatabaseStatics::GridStatics::MetaNameGridDisplayText), openvdb::TypedMetadata<FVoxelDatabaseText>(GridDisplayText));
            Grids.Add(OutGridId, GridPtr);
            IsGridAdded = true;
        }
        return IsGridAdded;
    }

    template<typename ValueType>
    VOXELMATEINLINE const ValueType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                return Accessor.getValue(Coord);
            }
        }

        const static ValueType ZeroValue = openvdb::zeroVal<ValueType>();
        return ZeroValue;
    }

    template<typename ValueType>
    VOXELMATEINLINE const ValueType& GetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, bool& OutIsVoxelActive) const
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                OutIsVoxelActive = Accessor.isValueOn(Coord);
                return Accessor.getValue(Coord);
            }
        }

        OutIsVoxelActive = false;
        const static ValueType ZeroValue = openvdb::zeroVal<ValueType>();
        return ZeroValue;
    }

    template<typename ValueType>
    VOXELMATEINLINE bool GetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord) const
    {
        bool IsActive = false;
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::ConstAccessor Accessor = TypedGridPtr->getConstAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                IsActive = Accessor.isValueOn(Coord);
            }
        }

        return IsActive;
    }

    template<typename ValueType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const ValueType& InVoxelValue)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                Accessor.setValueOnly(Coord, InVoxelValue);
            }
        }
    }

    template<typename ValueType>
    VOXELMATEINLINE void SetVoxelValue(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const ValueType& InVoxelValue, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
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

    template<typename ValueType>
    VOXELMATEINLINE void SetVoxelIsActive(const FGuid& GridId, const FIntVector& VoxelIndexCoord, const bool& InIsActive)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::Accessor Accessor = TypedGridPtr->getAccessor();
                const openvdb::Coord Coord(VoxelIndexCoord.X, VoxelIndexCoord.Y, VoxelIndexCoord.Z);
                Accessor.setActiveState(Coord, InIsActive);
            }
        }
    }

private:
    friend class FVoxelMateModule;

    UVoxelDatabase()
        : IsDatabaseInitialized(false)
    {
        VoxelDatabase = this;
        AddToRoot();
    }

    bool IsDatabaseInitialized;
    TMap<FGuid, FGridFactory::ValueTypePtr> Grids;
    TMap<FGuid, FMetaValueFactory::ValueTypePtr> Metadata;
};
