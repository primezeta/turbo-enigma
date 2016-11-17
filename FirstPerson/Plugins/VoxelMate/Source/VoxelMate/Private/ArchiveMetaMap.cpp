#include "VoxelMatePCH.h"
#include "ArchiveMetaMap.h"
#include "ArchiveMetaValue.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

FArchive& operator<<(FArchive& Ar, openvdb::MetaMap& MetaMap)
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
            FString MetaName;
            Ar << MetaName;
            check(MetaName.Len() > 0);

            MetaValueFactory.Serialize(Ar);
            try
            {
                if (MetaValueFactory.ValuePtr != nullptr)
                {
                    MetaMap.insertMeta(TCHAR_TO_UTF8(*MetaName), *MetaValueFactory.ValuePtr);
                }
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
            openvdb::Metadata::Ptr& MetaValuePtr = i->second;
            if (MetaValuePtr != nullptr)
            {
                //Serialize the meta name and value
                FString MetaName = UTF8_TO_TCHAR(i->first.c_str());
                Ar << MetaName;

                MetaValueFactory.ValuePtr = MetaValuePtr;
                MetaValueFactory.Serialize(Ar);
            }
        }
    }
    return Ar;
}