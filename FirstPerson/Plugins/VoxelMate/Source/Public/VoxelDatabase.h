#pragma once
#include "String.h"
#include "Archive.h"
#include "VoxelData.h"
#include "VoxelMateStreams.h"
#include "GridDescriptorInOut.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelDatabase, Log, All)

#undef verify
#undef check
#define verifyue4(expr)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } } 
#define checkue4(expr) { if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } }

#pragma pack(push,VoxelDatabaseInfoPack,1)
struct FVoxelDatabaseInfo
{
	typedef boost::shared_ptr<FVoxelDatabaseInfo> FPtr;

	FVoxelDatabaseInfo()
		: MagicNumber(0), MajorVersion(0), MinorVersion(0), SupportsPartialGridReading(false), FileCompression(EGridCompression::GridCompressionNone)
	{
		FMemory::Memset(UUID, 0, 16);
	}

	//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
	int64 MagicNumber;
	uint32 MajorVersion;
	uint32 MinorVersion;
	bool SupportsPartialGridReading;
	EGridCompression FileCompression;
	char UUID[16];
};
#pragma pack(pop,VoxelDatabaseInfoPack)

struct FVoxelDatabaseMetadataAttributes
{
	FVoxelDatabaseMetadataAttributes()
		: Name(TEXT("")), TypeID(TEXT("")), IsReadOnly(false)
	{}

	FVoxelDatabaseMetadataAttributes(const FString &name, const FString &typeID)
		: Name(name), TypeID(typeID), IsReadOnly(FVoxelDatabaseMetadataAttributes::IsMetadataReadOnly(name))
	{}

	FString ToString()
	{
		return FString::Printf(TEXT("%s:%s:%s"), *Name, *TypeID, IsReadOnly ? TEXT("Rx") : TEXT("RW"));
	}

	const FString Name;
	const FString TypeID;
	const bool IsReadOnly;

	static inline bool IsMetadataReadOnly(const FString &MetaName)
	{
		return IsMetadataReadOnly(std::string(TO_GRID_DATABASE_STRING(MetaName)));
	}

	static inline bool IsMetadataReadOnly(const std::string &MetaName)
	{
		const bool isReadOnly =
			MetaName == GridBaseStatics::MetaNameGridClass       ||
			MetaName == GridBaseStatics::MetaNameGridCreator     ||
			MetaName == GridBaseStatics::MetaNameGridName        ||
			MetaName == GridBaseStatics::MetaNameSaveHalfFloat   ||
			MetaName == GridBaseStatics::MetaNameIsLocalSpace    ||
			MetaName == GridBaseStatics::MetaNameVectorType      ||
			MetaName == GridBaseStatics::MetaNameFileBBoxMin     ||
			MetaName == GridBaseStatics::MetaNameFileBBoxMax     ||
			MetaName == GridBaseStatics::MetaNameFileCompression ||
			MetaName == GridBaseStatics::MetaNameFileMemBytes;
		return isReadOnly;
	}
};

class FVoxelDatabaseArchive : public FMemoryArchive, public FGridArchive, public boost::iostreams::stream<boost::iostreams::array>
{
public:
	//TODO
	//	Do not use FVoxelDatabaseInfo. Instead, provide a way to convert between openvdb .vdb file format and the UE4 archive format.
	//	Store metamap values in TArray or TMap. Whenever a metadata type is registered, as well as the current factory functor,
	//	store a callback function that will archive the container per type.
	//	Metamap values (file and grid meta) should be stored in regular memory.
	//	Only store the trees and tree buffers in memory mapped format.
	//	Use value obtained from Grid::memUsage() to initialize shared memory segments (which is stored as grid stats under META_FILE_MEM_BYTES)
	//	Create a configurable "radius" for grid regions. The total number of regions (current region + surrounding regions) define the shared memory segment size.
	//	The I/O buffer might be smaller than the total region. Far regions can be cached in memory but not synced to disk and the I/O buffer manipulated.
	//	Multiple groups of possibly disjoint regions could be active with separate I/O buffers.
	//	Any change to a region is communicated to the shared region.
	//	Could use 1-server-N-clients. Clients manage I/O but local changes to grid states are allowed/disallowed by the server and the server syncs changes to clients.
	//	Anyway, ignore client/server complications. Just design with extensibility in mind for future design.
	FVoxelDatabaseArchive(const FString &DatabaseName)
		: GridArchiveName(TO_GRID_DATABASE_STRING(DatabaseName))
	{
		////Register the standard grid and metadata types to enable the respective types to be serialized to and from shared memory
		//InitializeStandardTypes(SharedMemory);

		//const size_t PageSize = memory_manager::
		//boost::interprocess::mapped_region region()

		//Initially allocate enough memory to read in the vdb file header
			//SharedMemory(boost::interprocess::open_or_create, GridArchiveName, sizeof(FVoxelDatabaseInfo)),
			//MemoryBuffer(static_cast<char*>(SharedMemory.get_address()), SharedMemory.get_size()),
			//StreamBuffer(MemoryBuffer)
		//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
		//Header size is int64:magic number, 2*uint32:file version, char:has offsets flag, 16*char:128-bit boost::uuids::uuid
		//const boost::ulong_long_type FileHeaderSizeTest = sizeof(int64) + (2 * sizeof(uint32)) + sizeof(char) + (16 * sizeof(char));
		//const boost::ulong_long_type FileHeaderSize = sizeof(FVoxelDatabaseInfo);
		//checkue4(FileHeaderSizeTest == FileHeaderSize);
		////SharedMemory.allocate(FileHeaderSize); TODO: Need to allocate after open_or_create?
		//openvdb::io::setCurrentVersion(StreamBuffer);
	}

	template<typename MetaType>
	void RegisterMetaType()
	{
		//TODO for this and other registered types:
		//Move TFunction initializaton to GridDescriptorInOut.h and just call parameterless FMetadata<ValueType>::RegisterMeta() here
		//Also use TFunctionRef, referencing the static declaration of the tfunction
		if (!FMetadata<MetaType>::IsRegisteredType<MetaType>())
		{
			FMetadata<MetaType>::RegisterMeta<MetaType>();
		}
	}

	template<typename TreeType>
	void RegisterGridType()
	{
		if (!FGrid<TreeType>::IsRegistered<TreeType>())
		{
			//FGrid<TreeType> Grid;
			//Grid.fill()
			FGrid<TreeType>::RegisterGrid<TreeType>();
		}
	}

	template<typename MapType>
	void RegisterTransformMapType()
	{
		if (!FTransformMap<MapType>::IsRegistered<MapType>())
		{
			FTransformMap<MapType>::RegisterTransformMap<MapType>();
		}
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal, and Leaf
	*/
	template<typename DataType>
	void RegisterGrid3()
	{
		RegisterGridType<FTree3<DataType>>();
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, and Leaf
	*/
	template<typename DataType>
	void RegisterGrid4()
	{
		RegisterGridType<FTree4<DataType>>();
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, Internal2, and Leaf
	*/
	template<typename DataType>
	void RegisterGrid5()
	{
		RegisterGridType<FTree5<DataType>>();
	}

	/* Register a standard grid type (i.e. Grid4)
	*/
	template<typename DataType>
	void RegisterStandardGrid()
	{
		RegisterGrid4<DataType>();
	}

	void InitializeStandardTypes()
	{
		//Register the standard grid and metadata types
		GridBaseStatics::ClearRegistry();
		RegisterStandardGrid<bool>();
		RegisterStandardGrid<float>();
		RegisterStandardGrid<double>();
		RegisterStandardGrid<int32>();
		RegisterStandardGrid<int64>();
		RegisterStandardGrid<FGridDatabaseString>();
		RegisterStandardGrid<FVector2D>();
		RegisterStandardGrid<FVector>();
		RegisterStandardGrid<FVector4>();
		RegisterStandardGrid<FIntVector2>();
		RegisterStandardGrid<FIntVector>();
		RegisterStandardGrid<FIntVector4>();
		RegisterStandardGrid<FUintVector4>();

		openvdb::Metadata::clearRegistry();
		RegisterMetaType<bool>();
		RegisterMetaType<double>();
		RegisterMetaType<float>();
		RegisterMetaType<int32>();
		RegisterMetaType<int64>();
		RegisterMetaType<FGridDatabaseString>();
		RegisterMetaType<FIntVector2>();
		RegisterMetaType<FVector2D>();
		RegisterMetaType<FIntVector>();
		RegisterMetaType<FVector>();
		RegisterMetaType<FMatrix>();
		RegisterMetaType<FPointIndex32>();

		openvdb::math::MapRegistry::clear();
		RegisterTransformMapType<openvdb::math::AffineMap>();
		RegisterTransformMapType<openvdb::math::UnitaryMap>();
		RegisterTransformMapType<openvdb::math::ScaleMap>();
		RegisterTransformMapType<openvdb::math::UniformScaleMap>();
		RegisterTransformMapType<openvdb::math::TranslationMap>();
		RegisterTransformMapType<openvdb::math::ScaleTranslateMap>();
		RegisterTransformMapType<openvdb::math::UniformScaleTranslateMap>();
		RegisterTransformMapType<openvdb::math::NonlinearFrustumMap>();
	}

	bool ContainsGrid(const FString& gridName) const
	{
		bool containsGrid = false;
		if (GridArchiveIsOpen)
		{
			containsGrid = (FindGridDescriptor(gridName) != GridDescriptors.end());
		}
		return containsGrid;
	}

	bool GetFileMetadataAttributes(TArray<FVoxelDatabaseMetadataAttributes> &OutAttributes) const
	{
		bool areAnyAttributesValid = false;
		for (auto i = DatabaseMetadata.beginMeta(); i != DatabaseMetadata.endMeta(); ++i)
		{
			const FString metaNameStr = FROM_GRID_DATABASE_STRING(i->first);
			const FString metaTypeStr = FROM_GRID_DATABASE_STRING(i->second->typeName());
			OutAttributes.Add(FVoxelDatabaseMetadataAttributes(metaNameStr, metaTypeStr));
		}
		areAnyAttributesValid = DatabaseMetadata.metaCount() > 0;
		return areAnyAttributesValid;
	}

	template<typename ValueType>
	bool GetFileMetadataValue(const FString &MetaName, ValueType &OutValue, FVoxelDatabaseMetadataAttributes &OutAttributes) const
	{
		bool isMetadataValueFound = false;
		FMetadata<ValueType>::FPtr metaPtr = DatabaseMetadata.getMetadata<ValueType>(TO_GRID_DATABASE_STRING(MetaName));
		const FGridDatabaseString typeNameStdStr = metaPtr ? metaPtr->typeName() : FMetadataUnknown().typeName();
		FVoxelDatabaseMetadataAttributes MetaAttributes(MetaName, FROM_GRID_DATABASE_STRING(typeNameStdStr));
		if (metaPtr)
		{
			OutValue = *metaPtr;
			OutAttributes = MetaAttributes;
			isFileMetadataFound = true;
		}
		UE_LOG(LogVoxelDatabase, Verbose, TEXT("%s"), MetaAttributes.ToString());

		isMetadataValueFound = metaPtr != nullptr;
		return isMetadataValueFound;
	}

	template<typename ValueType>
	bool GetFileMetadataValue(const FString &MetaName, ValueType &OutValue) const
	{
		FVoxelDatabaseMetadataAttributes MetaAttributes;
		return GetFileMetadataValue<ValueType>(MetaName, OutValue, MetaAttributes);
	}

	/* If the database is not already open, reset the stream buffer to the beginning of the file and read the header and grid descriptors */
	bool OpenDatabase()
	{
		if (GridArchiveIsOpen)
		{
			return true;
		}
		else if (BytesCount > UINT32_MAX)
		{
			return false; //TODO Logging, error handling, etc
		}
		
		bool isOpen = false;
		boost::iostreams::stream<boost::iostreams::array>& Stream = *this;

		//Preallocate memory, associate the stream buffer to the memory, then open the stream on top of the stream buffer
		Bytes.SetNumUninitialized(BytesCount, false);
		BytesAccessor.reset(new boost::iostreams::array(Bytes.GetData(), Bytes.GetData() + Bytes.Num()));
		Stream.open(*BytesAccessor);

		//Read the file header as the first item in the buffer
		Archive::readHeader(Stream);
		DatabaseFileHeader = FVoxelDatabaseInfo::FPtr(reinterpret_cast<FVoxelDatabaseInfo*>(Bytes.GetData()));

		//Initialize I/O stream metadata
		const bool transfer = false;
		StreamMetadataPtr.reset(new FStreamMetadata());
		GridIOStatics::SetStreamMetadataPtr(Stream, StreamMetadataPtr, transfer);
		Archive::setFormatVersion(Stream);
		Archive::setLibraryVersion(Stream);
		Archive::setDataCompression(Stream);

		//DO NOT set a mapped file.
		//If a mapped file is set, leaf nodes would attempt to read in their buffers via the boost mapped file contained in the unchangeable Impl.
		//In addition to not setting the mapped file, OPENVDB_2_ABI_COMPATIBLE must be defined to avoid other mapped file operations under the covers of openvdb::io.
		//The mapped file pointer is null by default as long as openvdb::io::setMappedFilePtr isn't called.
		//For reference, if a mapped file was desired and #ifndef OPENVDB_2_ABI_COMPATIBLE, this would be the next call:
		//openvdb::io::setMappedFilePtr(StreamBuffer, mFileMapping);

		//Read the file-level metadata
		DatabaseMetadata.clearMetadata();
		DatabaseMetadata.readMeta(Stream);

		//Read the number of grids then for each grid read just the grid descriptor (not grid data)
		NumGrids = readGridCount(Stream);
		GridDescriptors.clear();
		for (int32 i = 0; i < NumGrids; ++i)
		{
			FGridDescriptor::ReadAndInsertGridDescriptor(Stream, GridDescriptors);
		}

		GridArchiveIsOpen = isOpen;
		return isOpen;
	}

	/* Return the database descriptor (i.e. file metadata) */
	bool GetDatabaseInfo(FVoxelDatabaseInfo &OutVoxelDatabaseInfo) const
	{
		if (GridArchiveIsOpen)
		{
			OutVoxelDatabaseInfo = *DatabaseFileHeader;
		}
		return GridArchiveIsOpen;
	}

protected:
	FGridDescriptorNameMapCIter FindGridDescriptor(const FString &gridName) const
	{
		const FGridName name = TO_GRID_DATABASE_STRING(gridName);
		const FGridName uniqueName = FGridDescriptor::StringAsUniqueName(name);

		// Find all descriptors with the given grid name.
		std::pair<FGridDescriptorNameMapCIter, FGridDescriptorNameMapCIter> range = GridDescriptors.equal_range(name);

		if (range.first == range.second)
		{
			// If no descriptors were found with the given grid name, the name might have
			// a suffix ("name[N]").  In that case, remove the "[N]" suffix and search again.
			range = GridDescriptors.equal_range(FGridDescriptor::StripSuffix(uniqueName).c_str());
		}

		const size_t count = size_t(std::distance(range.first, range.second));
		if (count > 1 && name == uniqueName)
		{
			UE_LOG(LogVoxelDatabase, Error, TEXT("%s has more than one grid named %s"), *GetArchiveName(), *gridName);
		}

		FGridDescriptorNameMapCIter foundGridDescriptor = GridDescriptors.end();
		if (count > 0)
		{
			if (name == uniqueName)
			{
				// If the given grid name is unique or if no "[N]" index was given,
				// use the first matching descriptor.
				foundGridDescriptor = range.first;
			}
			else
			{
				// If the given grid name has a "[N]" index, find the descriptor
				// with a matching unique name.
				for (FGridDescriptorNameMapCIter i = range.first; i != range.second; ++i)
				{
					const FGridName candidateName = i->second.UniqueName;
					if (candidateName == uniqueName || candidateName == name)
					{
						foundGridDescriptor = i;
						break;
					}
				}
			}
		}
		return foundGridDescriptor;
	}

protected:
	TArray<char> Bytes;
	boost::shared_ptr<boost::iostreams::array> BytesAccessor;
	size_t BytesCount;
	TMap<FGridName, size_t> MaxGridSizeByType;

	const FGridDatabaseString GridArchiveName;
	bool GridArchiveIsOpen;
	int32 NumGrids;

	FGridDescriptorNameMap GridDescriptors;
	FVoxelDatabaseInfo::FPtr DatabaseFileHeader;
	FMetaMap DatabaseMetadata;
	FStreamMetadata::FPtr StreamMetadataPtr;
};
