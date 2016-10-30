#include "VoxelMatePrivatePCH.h"
#include "ArchiveMetaValue.h"

TArray<FString> FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames;

void FMetaValueFactory::Serialize(FArchive& Ar)
{
    if (Ar.IsLoading())
    {
        const FString TypeName = EnumValueToString<EVoxelDatabaseType>(Type);
        ValuePtr = FMetaValueFactory::Create(TypeName);
    }

    check(ValuePtr != nullptr);
    openvdb::Metadata& Value = *ValuePtr;
    Ar << Value;
}

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
