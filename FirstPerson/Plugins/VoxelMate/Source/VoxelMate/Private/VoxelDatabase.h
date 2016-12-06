#pragma once
#include "ArchiveGrid.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseProxy.h"
#include "VoxelValueSource.h"
#include "GridCoordinateTransforms.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <vector>
#include <openvdb/tools/ValueTransformer.h>
#include "VoxelDatabase.generated.h"

//TODO Use FText for any strings displayed to the user
//see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/FText/
//DECLARE_MULTICAST_DELEGATE_OneParam(FVoxelDatabaseOnMetadataChanged, class UVoxelDatabaseProxy*, const FGuid&);

template<typename ValueType>
struct ModifyValueOp
{
    const ValueType& InValue;

    ModifyValueOp(const ValueType& Value)
        : InValue(Value)
    {}

    VOXELMATEINLINE void operator()(ValueType& OutValue) const
    {
        OutValue = InValue;
    }
};

template<EVoxelIterator Iter, typename GridType>
struct VoxelIteratorAdaptor;

template<typename GridType>
struct VoxelIteratorAdaptor<EVoxelIterator::InactiveVoxelsIter, GridType>
{
    typedef typename GridType::ValueOffIter Type;
    VOXELMATEINLINE static Type& SelectIter(GridType& Grid)
    {
        return Grid.beginValueOff();
    }
};

template<typename GridType>
struct VoxelIteratorAdaptor<EVoxelIterator::ActiveVoxelsIter, GridType>
{
    typedef typename GridType::ValueOnIter Type;
    VOXELMATEINLINE static Type& SelectIter(GridType& Grid)
    {
        return Grid.beginValueOn();
    }
};

template<typename GridType>
struct VoxelIteratorAdaptor<EVoxelIterator::AllVoxelsIter, GridType>
{
    typedef typename GridType::ValueAllIter Type;
    VOXELMATEINLINE static Type& SelectIter(GridType& Grid)
    {
        return Grid.beginValueAll();
    }
};

template<typename IterT, typename VoxelT, typename ValueSourceT>
struct TSetValuesOp
{
    typedef typename VoxelT VoxelType;
    typedef typename VoxelT::ValueType ValueType;
    typedef typename IterT IterType;
    typedef typename ValueSourceT ValueSourceType;

    const ValueSourceType &ValueSource;
    openvdb::math::Transform &CoordinateTransform;

    TSetValuesOp(const ValueSourceType &VoxelValueSource, openvdb::math::Transform &Transform)
        : ValueSource(VoxelValueSource), CoordinateTransform(Transform)
    {}

    VOXELMATEINLINE void operator()(const IterType& iter) const
    {
        check(iter.isVoxelValue());
        const openvdb::Coord &Coord = iter.getCoord();
        const openvdb::Vec3d &Xyz = CoordinateTransform.indexToWorld(Coord);
        ValueType Value;
        ValueSource.GetValue(Xyz.x(), Xyz.y(), Xyz.z(), Value);
        iter.modifyValue<ModifyValueOp<VoxelType>>(ModifyValueOp<VoxelType>(Value));
    }
};

UCLASS(ClassGroup = VoxelMate, NotPlaceable, NotBlueprintable, CustomConstructor)
class UVoxelDatabase : public UObject
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

    template<typename VoxelType, typename ValueSourceType, EVoxelIterator VoxelIterType>
    VOXELMATEINLINE void SetVoxelValuesFromSource(const FGuid& GridId, const ValueSourceType &ValueSource)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>::Ptr TypedGridPtr = openvdb::gridPtrCast<openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>>(*FindGrid);
            if (TypedGridPtr)
            {
                typedef VoxelIteratorAdaptor<VoxelIterType, openvdb::Grid<openvdb::tree::Tree4<VoxelType>::Type>> IterAdaptor;
                typedef IterAdaptor::Type IterType;
                typedef TSetValuesOp<IterType, VoxelType, ValueSourceType> OpType;

                OpType SetValuesOp(ValueSource, TypedGridPtr->transform());
                openvdb::tools::foreach<IterType, OpType>(IterAdaptor::SelectIter(*TypedGridPtr), SetValuesOp);
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    template<typename VoxelType>
    VOXELMATEINLINE void FillGrid(const FGuid& GridId, const FIntVector& StartIndexCoord, const FIntVector& FillDimensions, const VoxelType& InValue, const bool &InIsActive, const bool &InVoxelizeActiveTilesAfterFill)
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
                const openvdb::Coord FillEnd(FillStart.x() + FillDimensions.X - 1, FillStart.y() + FillDimensions.Y - 1, FillStart.z() + FillDimensions.Z - 1);
                const openvdb::CoordBBox FillBBox(FillStart, FillEnd);
                TypedGridPtr->fill(FillBBox, InValue, InIsActive);

                if (InVoxelizeActiveTilesAfterFill)
                {
                    TypedGridPtr->tree().voxelizeActiveTiles();
                }
            }
            else
            {
                //TODO log error (grid types mismatched)
                check(false); //TODO handle error
            }
        }
    }

    template<typename MetadataType>
    bool AddMetadata(FGuid& OutMetadataId)
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

    void SetCoordinateTransform(const FGuid& GridId, const FAffineCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const FVector4 &Col0 = InCoordinateTransform.Matrix.GetColumn(0);
            const FVector4 &Col1 = InCoordinateTransform.Matrix.GetColumn(1);
            const FVector4 &Col2 = InCoordinateTransform.Matrix.GetColumn(2);
            const openvdb::math::Mat3d AffineMatrix(
                Col0.X, Col1.X, Col2.X,
                Col0.Y, Col1.Y, Col2.Y,
                Col0.Z, Col1.Z, Col2.Z
            );
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::AffineMap(AffineMatrix)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUnitaryCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            FVector Axis = FVector::ZeroVector;
            float Angle = 0.0f;
            InCoordinateTransform.Quat.ToAxisAndAngle(Axis, Angle);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UnitaryMap(openvdb::Vec3d(Axis.X, Axis.Y, Axis.Z), Angle)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Scale(InCoordinateTransform.ScaleVec.X, InCoordinateTransform.ScaleVec.Y, InCoordinateTransform.ScaleVec.Z);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleMap(Scale)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const float &Scale(InCoordinateTransform.ScaleValue);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleMap(Scale)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Translation(InCoordinateTransform.TranslationVec.X, InCoordinateTransform.TranslationVec.Y, InCoordinateTransform.TranslationVec.Z);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::TranslationMap(Translation)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Scale(InCoordinateTransform.ScaleVec.X, InCoordinateTransform.ScaleVec.Y, InCoordinateTransform.ScaleVec.Z);
            const openvdb::Vec3d Translation(InCoordinateTransform.TranslationVec.X, InCoordinateTransform.TranslationVec.Y, InCoordinateTransform.TranslationVec.Z);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::ScaleTranslateMap(Scale, Translation)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FUniformScaleTranslationCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const float &Scale = InCoordinateTransform.ScaleValue;
            const openvdb::Vec3d Translation(InCoordinateTransform.TranslationVec.X, InCoordinateTransform.TranslationVec.Y, InCoordinateTransform.TranslationVec.Z);
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::UniformScaleTranslateMap(Scale, Translation)))));
        }
    }

    void SetCoordinateTransform(const FGuid& GridId, const FNonlinearFrustumCoordinateTransform& InCoordinateTransform)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Box0(InCoordinateTransform.Box.Min.X, InCoordinateTransform.Box.Min.Y, InCoordinateTransform.Box.Min.Z);
            const openvdb::Vec3d Box1(InCoordinateTransform.Box.Max.X, InCoordinateTransform.Box.Max.Y, InCoordinateTransform.Box.Max.Z);
            const openvdb::BBoxd BoundingBox(Box0 < Box1 ? Box0 : Box1, Box0 > Box1 ? Box0 : Box1);
            const float &Taper = InCoordinateTransform.Taper;
            const float &Depth = InCoordinateTransform.Depth;
            (*FindGrid)->setTransform(openvdb::math::Transform::Ptr(new openvdb::math::Transform(openvdb::math::MapBase::Ptr(new openvdb::math::NonlinearFrustumMap(BoundingBox, Taper, Depth)))));
        }
    }

    void GridRotation(const FGuid& GridId, ETransformOp Op, float Radians, EAxis::Type Axis)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Axis RotationAxis =
                Axis == EAxis::X ? openvdb::math::Axis::X_AXIS :
                Axis == EAxis::Y ? openvdb::math::Axis::Y_AXIS :
                openvdb::math::Axis::Z_AXIS;

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preRotate(Radians, RotationAxis);
            }
            else
            {
                (*FindGrid)->transform().postRotate(Radians, RotationAxis);
            }
        }
    }

    void GridTranslation(const FGuid& GridId, ETransformOp Op, const FVector &InTranslation)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Translation(InTranslation.X, InTranslation.Y, InTranslation.Z);

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preTranslate(Translation);
            }
            else
            {
                (*FindGrid)->transform().postTranslate(Translation);
            }
        }
    }

    void GridScale(const FGuid& GridId, ETransformOp Op, const FVector &InScale)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::Vec3d Scale(InScale.X, InScale.Y, InScale.Z);

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preScale(Scale);
            }
            else
            {
                (*FindGrid)->transform().postScale(Scale);
            }
        }
    }

    void GridUniformScale(const FGuid& GridId, ETransformOp Op, float Scale)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preScale(Scale);
            }
            else
            {
                (*FindGrid)->transform().postScale(Scale);
            }            
        }
    }

    void GridShear(const FGuid& GridId, ETransformOp Op, float Shear, EAxis::Type Axis0, EAxis::Type Axis1)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Axis ShearAxis0 =
                Axis0 == EAxis::X ? openvdb::math::Axis::X_AXIS :
                Axis0 == EAxis::Y ? openvdb::math::Axis::Y_AXIS :
                openvdb::math::Axis::Z_AXIS;
            const openvdb::math::Axis ShearAxis1 =
                Axis1 == EAxis::X ? openvdb::math::Axis::X_AXIS :
                Axis1 == EAxis::Y ? openvdb::math::Axis::Y_AXIS :
                openvdb::math::Axis::Z_AXIS;

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preShear(Shear, ShearAxis0, ShearAxis1);
            }
            else
            {
                (*FindGrid)->transform().postShear(Shear, ShearAxis0, ShearAxis1);
            }
        }
    }

    void GridMatrix4dMultiply(const FGuid& GridId, ETransformOp Op, const FPlane &InX, const FPlane &InY, const FPlane &InZ, const FPlane &InW)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Mat4d Matrix(
                InX.X, InX.Y, InX.Z, InX.W,
                InY.X, InY.Y, InY.Z, InY.W,
                InZ.X, InZ.Y, InZ.Z, InZ.W,
                InW.X, InW.Y, InW.Z, InW.W
            );

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preMult(Matrix);
            }
            else
            {
                (*FindGrid)->transform().postMult(Matrix);
            }            
        }
    }

    void GridMatrix3dMultiply(const FGuid& GridId, ETransformOp Op, const FVector& InX, const FVector& InY, const FVector& InZ)
    {
        const FGridFactory::ValueTypePtr* FindGrid = Grids.Find(GridId);
        if (FindGrid != nullptr)
        {
            const openvdb::math::Mat3d Matrix(
                InX.X, InX.Y, InX.Z,
                InY.X, InY.Y, InY.Z,
                InZ.X, InZ.Y, InZ.Z
            );

            if (Op == ETransformOp::PreOp)
            {
                (*FindGrid)->transform().preMult(Matrix);
            }
            else
            {
                (*FindGrid)->transform().postMult(Matrix);
            }
        }
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
