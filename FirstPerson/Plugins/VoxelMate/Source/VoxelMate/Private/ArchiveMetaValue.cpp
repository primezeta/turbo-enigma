#include "VoxelMatePCH.h"
#include "EngineGridTypes.h"
#include "ArchiveMetaValue.h"
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

bool FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = false;
TArray<FString> FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames;

bool FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsRegisteredType(const FString& TypeName)
{
    return openvdb::Metadata::isRegisteredType(TCHAR_TO_UTF8(*TypeName));
}

FVoxelDatabaseTypeFactory<openvdb::Metadata>::ValueTypePtr FVoxelDatabaseTypeFactory<openvdb::Metadata>::CreateType(const FString& TypeName)
{
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::ValueTypePtr MetaValuePtr = nullptr;
    try
    {
        MetaValuePtr = openvdb::Metadata::createMetadata(TCHAR_TO_UTF8(*TypeName));
    }
    catch (const openvdb::LookupError& e)
    {
        (void)e; //TODO log error (metadata type not registered)
    }
    return MetaValuePtr;
}

void FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisterSupportedTypes()
{
    if (FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized)
    {
        return;
    }
    openvdb::Metadata::clearRegistry();
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::RegisteredTypeNames.Empty();
    FMetaValueFactory::Register<FVoxelDatabaseBoolMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseFloatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseDoubleMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseUInt8Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseUInt16Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseUInt32Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseUInt64Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt8Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt16Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt32Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt64Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseVectorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseVector4Meta>();
    FMetaValueFactory::Register<FVoxelDatabaseVector2DMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseColorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseLinearColorMeta>();
    FMetaValueFactory::Register<FVoxelDatabasePackedNormalMeta>();
    FMetaValueFactory::Register<FVoxelDatabasePackedRGB10A2NMeta>();
    FMetaValueFactory::Register<FVoxelDatabasePackedRGBA16NMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseIntPointMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseIntVectorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseFloatRangeBoundMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseFloatRangeMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt32RangeBoundMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt32RangeMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseFloatIntervalMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInt32IntervalMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseTwoVectorsMeta>();
    FMetaValueFactory::Register<FVoxelDatabasePlaneMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseRotatorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseQuatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseBoxMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseBox2DMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseBoxSphereBoundsMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseOrientedBoxMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseMatrixMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointFloatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveFloatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointVector2DMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveVector2DMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointVectorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveVectorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointQuatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveQuatMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointTwoVectorsMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveTwoVectorsMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurvePointLinearColorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseInterpCurveLinearColorMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseTransformMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseRandomStreamMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseDateTimeMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseTimespanMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseStringAssetReferenceMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseStringClassReferenceMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseStringMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseNameMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseTextMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseAffineMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseUnitaryMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseScaleMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseUniformScaleMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseTranslationMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseScaleTranslationMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseUniformScaleTranslationMapMetadataMeta>();
    FMetaValueFactory::Register<FVoxelDatabaseNonlinearFrustumMapMetadataMeta>();
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = true;
}

void FVoxelDatabaseTypeFactory<openvdb::Metadata>::UnregisterSupportedTypes()
{
    FVoxelDatabaseTypeFactory<openvdb::Metadata>::IsInitialized = false;
}

FArchive& operator<<(FArchive& Ar, FMetaValueFactory::ValueTypePtr& MetaValuePtr)
{
    FString TypeName;
    if (Ar.IsLoading())
    {
        Ar << TypeName;
        MetaValuePtr = FMetaValueFactory::CreateType(TypeName);
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
