#include "VoxelMatePCH.h"
#include "ArchiveMetaValue.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames;

FArchive& operator<<(FArchive& Ar, FMetaValueFactory::ValueTypePtr& MetaValuePtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        MetaValuePtr = FMetaValueFactory::Create(TypeName);
        if (MetaValuePtr != nullptr)
        {
            Ar << *MetaValuePtr;
        }
        //else {//TODO: need to seek past the unregistered data?}
    }
    else if (MetaValuePtr != nullptr)
    {
        TypeName = UTF8_TO_TCHAR(MetaValuePtr->typeName().c_str());
        Ar << TypeName;
        Ar << *MetaValuePtr;
    }
    return Ar;
}

FArchive& operator<<(FArchive& Ar, openvdb::Metadata& MetaValue)
{
    //A serialized meta value is a non-empty meta name, a 4-byte size value, and then the meta value byte array
    const openvdb::Index32 MetaValueSize = MetaValue.size();
    const openvdb::Index32 DataArraySize = MetaValueSize + sizeof(openvdb::Index32);

    TArray<ANSICHAR> DataBytes;
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
    ANSICHAR* BufferStart = DataBytes.GetData();
    ANSICHAR* BufferEnd = DataBytes.GetData() + DataArraySize - 1;
    boost::iostreams::array BufferDevice(BufferStart, BufferEnd);
    boost::iostreams::stream<boost::iostreams::array> IOStream;

    //TODO any exceptions to handle?
    IOStream.open(BufferDevice);
    if (IsLoading)
    {
        //Set the metadata value by reading from the tarray via the i/o stream
        MetaValue.read(static_cast<std::istream&>(IOStream));
    }
    else
    {
        //Write the metadata value to the tarray via the i/o stream
        MetaValue.write(static_cast<OutputStreamType&>(IOStream));

        //Serialize the tarray
        DataBytes.BulkSerialize(Ar);
    }
    IOStream.close();

    return Ar;
}
