#pragma once
#include "ArchiveMetaValue.h"
#include "openvdb/openvdb.h"

struct FMetaMapFactory
{
    template<typename MetaType>
    FORCEINLINE_DEBUGGABLE static void InsertMeta(openvdb::MetaMap& MetaMap, const openvdb::Name& MetaName, const MetaType& MetaValue)
    {
        try
        {
            MetaMap.insertMeta(MetaName, openvdb::TypedMetadata<MetaType>(MetaValue));
        }
        catch (const openvdb::ValueError &e)
        {
            //TODO log error (meta name is invalid i.e. size 0)
            (void)e;
        }
        catch (const openvdb::KeyError &e)
        {
            //TODO log error (meta key is invalid i.e. mismatched value types)
            (void)e;
        }
    }

    template<typename MetaType>
    FORCEINLINE_DEBUGGABLE static void InsertMeta(openvdb::MetaMap& MetaMap, const FString& MetaName, const MetaType& MetaValue)
    {
        FMetaMapFactory::InsertMeta<MetaType>(MetaMap, openvdb::Name(TCHAR_TO_UTF8(*MetaName)), MetaValue);
    }

    template<typename MetaType>
    FORCEINLINE_DEBUGGABLE static void InsertGridMeta(openvdb::GridBase& Grid, const openvdb::Name& MetaName, const MetaType& MetaValue)
    {
        openvdb::MetaMap& GridMetaMap = Grid;
        FMetaMapFactory::InsertMeta<MetaType>(GridMetaMap, MetaName, MetaValue);
    }

    template<typename MetaType>
    FORCEINLINE_DEBUGGABLE static void InsertGridMeta(openvdb::GridBase& Grid, const FString& MetaName, const MetaType& MetaValue)
    {
        openvdb::MetaMap& GridMetaMap = Grid;
        FMetaMapFactory::InsertMeta<MetaType>(GridMetaMap, MetaName, MetaValue);
    }
};
