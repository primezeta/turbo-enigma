#pragma once
#include "EngineMinimal.h"
#include "VoxelDatabaseTypeFactory.h"

FORCEINLINE static FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue);

struct FMetaValueFactory : public FVoxelDatabaseTypeFactory<openvdb::Metadata>
{
    virtual void Serialize(FArchive& Ar) override
    {
        if (Ar.IsLoading())
        {
            ValuePtr = FMetaValueFactory::Create(TypeName);
        }

        check(ValuePtr != nullptr);
        openvdb::Metadata& Value = *ValuePtr;
        Ar << Value;
    }

    template<typename MetaType>
    static inline void RegisterMetaType()
    {
        if (!IsRegistered(openvdb::TypedMetadata<MetaType>::staticTypeName()))
        {
            Register(openvdb::TypedMetadata<MetaType>::staticTypeName(), openvdb::TypedMetadata<MetaType>::createMetadata);
            FMetaValueFactory::RegisteredTypeDisplayNames.Add(VoxelDatabaseTypeNameDisplay<MetaType>());
        }
    }

    static ValueTypePtr Create(const openvdb::Name& TypeName)
    {
        return openvdb::Metadata::createMetadata(TypeName);
    }

    static void Register(const openvdb::Name& TypeName, openvdb::Metadata::Ptr(*createMetadata)())
    {
        openvdb::Metadata::registerType(TypeName, createMetadata);
    }

    static void Unregister(const openvdb::Name& TypeName)
    {
        openvdb::Metadata::unregisterType(TypeName);
    }

    static bool IsRegistered(const openvdb::Name& TypeName)
    {
        return openvdb::Metadata::isRegisteredType(TypeName);
    }

    static void ClearRegistry()
    {
        openvdb::Metadata::clearRegistry();
    }
};

FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue)
{
    //A serialized meta value is a non-empty meta name, a 4-byte size value, and then the meta value byte array
    const openvdb::Index32 MetaValueSize = MetaValue.size();
    const openvdb::Index32 DataArraySize = MetaValueSize + sizeof(openvdb::Index32);

    TArray<char> DataBytes;
    const bool IsLoading = Ar.IsLoading();
    if (IsLoading)
    {
        DataBytes.BulkSerialize(Ar);
    }
    else
    {
        DataBytes.SetNumUninitialized(DataArraySize, false);
    }
    check(DataArraySize > 0 && DataBytes.Num() == DataArraySize);

    //Create an i/o stream for reading/writing the tarray
    char* BufferStart = DataBytes.GetData();
    char* BufferEnd = DataBytes.GetData() + DataArraySize - 1;
    boost::iostreams::array BufferDevice(BufferStart, BufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    //TODO any exceptions to handle?
    IOStream.open(BufferDevice);
    if (IsLoading)
    {
        //Set the metadata value by reading from the tarray via the i/o stream
        MetaValue.read(IOStream);
    }
    else
    {
        //Write the metadata value to the tarray via the i/o stream
        MetaValue.write(static_cast<std::ostream&>(IOStream));

        //Serialize the tarray
        DataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
