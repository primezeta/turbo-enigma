#pragma once
#include <boost/shared_ptr.hpp>

template<typename VoxelDatabaseType>
struct FVoxelDatabaseTypeFactory
{
    typedef typename VoxelDatabaseType ValueType;
    typedef typename boost::shared_ptr<ValueType> ValueTypePtr;

    static TArray<FString> RegisteredTypeNames;
    static TArray<FString> RegisteredTypeDisplayNames;
    ValueTypePtr ValuePtr;
};
