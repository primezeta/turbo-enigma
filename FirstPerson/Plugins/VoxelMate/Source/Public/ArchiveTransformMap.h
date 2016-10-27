#pragma once
#include "EngineMinimal.h"
#include "ArchiveMetaMap.h" //TODO make a general .h for archive functions
#include "VoxelDatabaseTypeFactory.h"

FORCEINLINE static FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap);

struct FTransformMapFactory : public FVoxelDatabaseTypeFactory<openvdb::math::MapBase>
{
    virtual void Serialize(FArchive& Ar) override
    {
        if (Ar.IsLoading())
        {
            ValuePtr = FTransformMapFactory::Create(TypeName);
        }

        check(ValuePtr != nullptr);
        openvdb::math::MapBase& Value = *ValuePtr;
        Ar << Value;
    }

    template<typename MapType>
    static inline void RegisterTransformMapType()
    {
        if (!IsRegistered(MapType::mapType()))
        {
            Register(MapType::mapType(), MapType::create);
            FTransformMapFactory::RegisteredTypeDisplayNames.Add(VoxelDatabaseTypeNameDisplay<MapType>());
        }
    }

    static ValueTypePtr Create(const openvdb::Name& TypeName)
    {
        return openvdb::math::MapRegistry::createMap(TypeName);
    }

    static void Register(const openvdb::Name& TypeName, openvdb::math::MapBase::MapFactory Factory)
    {
        openvdb::math::MapRegistry::registerMap(TypeName, Factory);
    }

    static void Unregister(const openvdb::Name& TypeName)
    {
        openvdb::math::MapRegistry::unregisterMap(TypeName);
    }

    static bool IsRegistered(const openvdb::Name& TypeName)
    {
        return openvdb::math::MapRegistry::isRegistered(TypeName);
    }

    static void ClearRegistry()
    {
        openvdb::math::MapRegistry::clear();
    }
};

FArchive& operator<<(FArchive& Ar, openvdb::math::MapBase& TransformMap)
{
    //De/serialize the tarray containing the transform map
    TArray<char> DataBytes;
    if (Ar.IsLoading())
    {
        DataBytes.BulkSerialize(Ar);
    }
    else
    {
        const int32 MapSize = VoxelDatabaseStatics::TransformMapStatics::SizeOfMap(TransformMap.type());
        DataBytes.SetNumUninitialized(MapSize > 0 ? MapSize : 0, false);
    }

    const openvdb::Index32 DataArraySize = DataBytes.Num();
    check(DataArraySize > 0);

    //Create an i/o stream for reading/writing the tarray
    char* BufferStart = DataBytes.GetData();
    char* BufferEnd = DataBytes.GetData() + DataArraySize - 1;
    boost::iostreams::array BufferDevice(BufferStart, BufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    //TODO any exceptions to handle?
    IOStream.open(BufferDevice);
    if (Ar.IsLoading())
    {
        TransformMap.read(IOStream);
    }
    else
    {
        TransformMap.write(IOStream);
        DataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
