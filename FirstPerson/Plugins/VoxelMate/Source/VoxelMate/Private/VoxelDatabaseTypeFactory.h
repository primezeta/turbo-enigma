#pragma once
#include <boost/shared_ptr.hpp>

template<typename VoxelDatabaseType>
struct FVoxelDatabaseTypeFactory
{
    typedef typename VoxelDatabaseType ValueType;
    typedef typename boost::shared_ptr<ValueType> ValueTypePtr;
    typedef typename boost::shared_ptr<const ValueType> ValueTypeConstPtr;
    typedef typename boost::weak_ptr<ValueType> ValueTypeWeakPtr;

    static TArray<FString> RegisteredTypeNames;
    static TArray<FString> RegisteredTypeDisplayNames;
    ValueTypePtr ValuePtr;
};
