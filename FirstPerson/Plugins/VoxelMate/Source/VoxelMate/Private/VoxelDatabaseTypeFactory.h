#pragma once
#include <boost/shared_ptr.hpp>

template<typename VoxelDatabaseType>
struct FVoxelDatabaseTypeFactory
{
    typedef typename VoxelDatabaseType ValueType;
    typedef typename boost::shared_ptr<ValueType> ValueTypePtr;
    typedef typename boost::shared_ptr<const ValueType> ValueTypeConstPtr;
    ValueTypePtr ValuePtr;

    static const TArray<FString>& GetRegisteredTypeNames()
    {
        return FVoxelDatabaseTypeFactory<VoxelDatabaseType>::RegisteredTypeNames;
    }

    static void RegisterSupportedTypes();
    static void UnregisterSupportedTypes();

protected:
    static bool IsInitialized;
    static TArray<FString> RegisteredTypeNames;
    static bool IsRegisteredType(const FString& TypeName);
    static ValueTypePtr CreateType(const FString& TypeName);
};
