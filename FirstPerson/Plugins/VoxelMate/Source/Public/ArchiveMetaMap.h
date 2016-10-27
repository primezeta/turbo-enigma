#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaValue.h"
#include "VoxelDatabaseTypeFactory.h"

FORCEINLINE FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap)
{
    FMetaValueFactory MetaValueFactory;
    uint64 NumMetadata = (uint64)MetaMap.metaCount();
    Ar << NumMetadata;

    if (Ar.IsLoading())
    {
        //Deserialize each meta value and insert it into the metamap
        MetaMap.clearMetadata();
        for (uint64 i = 0; i < NumMetadata; ++i)
        {
            //Deserialize the meta value
            openvdb::Name MetaName;
            Ar << MetaName;
            check(MetaName.size() > 0);

            MetaValueFactory.Serialize(Ar);
            try
            {
                MetaMap.insertMeta(MetaName, *MetaValueFactory.ValuePtr);
            }
            catch (const openvdb::ValueError& e)
            {
                //TODO log warning or error (metaname is not valid)
                (void)e.what();
            }
            catch (const openvdb::TypeError& e)
            {
                //TODO log warning or error (metadata already exists but typename does not match)
                (void)e.what();
            }
        }
    }
    else
    {
        //Serialize metadata for all non-null metadata items
        for (openvdb::MetaMap::MetaIterator i = MetaMap.beginMeta(); i != MetaMap.endMeta(); ++i)
        {
            //Serialize the meta name and value
            openvdb::Name MetaName = i->first;
            Ar << MetaName;

            openvdb::Metadata::Ptr& MetaValuePtr = i->second;
            MetaValueFactory.ValuePtr = MetaValuePtr;
            MetaValueFactory.Serialize(Ar);
        }
    }
    return Ar;
}
