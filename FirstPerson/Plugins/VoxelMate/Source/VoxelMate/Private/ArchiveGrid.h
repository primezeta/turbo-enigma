#pragma once
#include "EngineGridTypes.h"

#pragma warning(push)
#pragma warning(once:4211 4800 4503 4146)

struct FGridFactory : public FVoxelDatabaseTypeFactory<openvdb::GridBase>
{
    static TMap<FString, int32> CachedTopologySizeByTreeType;
    uint8 CompressionFlags;

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGridFactory::ValueTypePtr& GridPtr);
    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, openvdb::GridBase& Grid);

    template<typename ValueType>
    static void RegisterGridType()
    {
        if (!openvdb::Grid<openvdb::tree::Tree3<ValueType>::Type>::isRegistered())
        {
            const static openvdb::tree::Tree3<ValueType>::Type DummyTree3;
            openvdb::Grid<openvdb::tree::Tree3<ValueType>::Type>::registerGrid();
            FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(DummyTree3.type().c_str()));
        }

        if (!openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::isRegistered())
        {
            const static openvdb::tree::Tree4<ValueType>::Type DummyTree4;
            openvdb::Grid<openvdb::tree::Tree4<ValueType>::Type>::registerGrid();
            FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(DummyTree4.type().c_str()));
        }

        if (!openvdb::Grid<openvdb::tree::Tree5<ValueType>::Type>::isRegistered())
        {
            const static openvdb::tree::Tree5<ValueType>::Type DummyTree5;
            openvdb::Grid<openvdb::tree::Tree5<ValueType>::Type>::registerGrid();
            FGridFactory::RegisteredTypeNames.Add(UTF8_TO_TCHAR(DummyTree5.type().c_str()));
        }
    }

    static bool IsRegistered(const FString& TypeName)
    {
        return openvdb::GridBase::isRegistered(TCHAR_TO_UTF8(*TypeName));
    }

    static ValueTypePtr Create(const FString& TypeName)
    {
        ValueTypePtr GridPtr = nullptr;
        try
        {
            GridPtr = openvdb::GridBase::createGrid(TCHAR_TO_UTF8(*TypeName));
        }
        catch (const openvdb::LookupError& e)
        {
            (void)e; //TODO log error (grid type name is not registered)
        }
        return GridPtr;
    }

    static void ClearRegistry()
    {
        openvdb::GridBase::clearRegistry();
    }
};

#pragma warning(pop)