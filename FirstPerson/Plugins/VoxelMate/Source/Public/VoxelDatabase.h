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

struct FVoxelDatabaseMetadata
{
    FVoxelDatabaseMetadata(const FLibraryVersionId& libraryVersion, bool partialGridReading, EGridCompression fileCompression, const FGridDatabaseString& uuid)
		: LibraryVersion(libraryVersion), SupportsPartialGridReading(partialGridReading), FileCompression(fileCompression), UUID(uuid)
    {}

	//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
    FLibraryVersionId LibraryVersion;
    bool SupportsPartialGridReading;
    EGridCompression FileCompression;
    FGridDatabaseString UUID;
};

struct FVoxelFileMetadata
{
    FVoxelFileMetadata()
		: Name(TEXT("")), TypeID(TEXT("")), IsReadOnly(false)
	{}

    FVoxelFileMetadata(const FString &name, const FString &typeID)
		: Name(name), TypeID(typeID), IsReadOnly(FVoxelFileMetadata::IsMetadataReadOnly(name))
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

class FVoxelDatabaseArchive : public FMemoryArchive, public FGridArchive, public FStream<boost::iostreams::stream<boost::iostreams::array>>
{
public:
	//TODO
	//	Do not use FVoxelDatabaseMetadata. Instead, provide a way to convert between openvdb .vdb file format and the UE4 archive format.
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
		: GridArchiveName(DatabaseName)
	{
		if (FMemoryArchive::IsLoading())
		{
			BytesCount = FMemoryArchive::TotalSize();
		}
		else
		{
			BytesCount = 0;
		}

		////Register the standard grid and metadata types to enable the respective types to be serialized to and from shared memory
		//InitializeStandardTypes(SharedMemory);

		//const size_t PageSize = memory_manager::
		//boost::interprocess::mapped_region region()

		//Initially allocate enough memory to read in the vdb file header
			//SharedMemory(boost::interprocess::open_or_create, GridArchiveName, sizeof(FVoxelDatabaseMetadata)),
			//MemoryBuffer(static_cast<char*>(SharedMemory.get_address()), SharedMemory.get_size()),
			//StreamBuffer(MemoryBuffer)
		//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
		//Header size is int64:magic number, 2*uint32:file version, char:has offsets flag, 16*char:128-bit boost::uuids::uuid
		//const boost::ulong_long_type FileHeaderSizeTest = sizeof(int64) + (2 * sizeof(uint32)) + sizeof(char) + (16 * sizeof(char));
		//const boost::ulong_long_type FileHeaderSize = sizeof(FVoxelDatabaseMetadata);
		//checkue4(FileHeaderSizeTest == FileHeaderSize);
		////SharedMemory.allocate(FileHeaderSize); TODO: Need to allocate after open_or_create?
		//openvdb::io::setCurrentVersion(StreamBuffer);
	}

	virtual FString GetArchiveName() const override
	{
		return GridArchiveName;
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
            RegisteredMetadataTypeDisplayNames.Add(GridTypeNameDisplay<MetaType>());
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
            RegisteredGridTypeDisplayNames.Add(GridTypeNameDisplay<TreeType::ValueType>());
		}
	}

	template<typename MapType>
	void RegisterTransformMapType()
	{
		if (!FTransformMap<MapType>::IsRegistered<MapType>())
		{
			FTransformMap<MapType>::RegisterTransformMap<MapType>();
            RegisteredTransformMapTypeDisplayNames.Add(GridTypeNameDisplay<MapType>());
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
		RegisterStandardGrid<FVector2D>();
		RegisterStandardGrid<FVector>();
		RegisterStandardGrid<FVector4>();
		RegisterStandardGrid<FIntVector2>();
		RegisterStandardGrid<FIntVector>();
		RegisterStandardGrid<FIntVector4>();
		RegisterStandardGrid<FUintVector4>();
        RegisterStandardGrid<FPointIndex32>();
        RegisterStandardGrid<FPointIndex64>();
        RegisterStandardGrid<FGridDatabaseString>();

		MetadataStatics::ClearRegistry();
		RegisterMetaType<bool>();
		RegisterMetaType<double>();
		RegisterMetaType<float>();
		RegisterMetaType<int32>();
		RegisterMetaType<int64>();
		RegisterMetaType<FIntVector2>();
		RegisterMetaType<FVector2D>();
		RegisterMetaType<FIntVector>();
		RegisterMetaType<FVector>();
		RegisterMetaType<FMatrix>();
        RegisterMetaType<FPointIndex32>();
        RegisterMetaType<FPointIndex64>();
        RegisterMetaType<FGridDatabaseString>();

		TransformMapStatics::ClearRegistry();
		RegisterTransformMapType<FTransformAffineMap>();
		RegisterTransformMapType<FTransformUnitaryMap>();
		RegisterTransformMapType<FTransformScaleMap>();
		RegisterTransformMapType<FTransformUniformScaleMap>();
		RegisterTransformMapType<FTransformTranslationMap>();
		RegisterTransformMapType<FTransformScaleTranslateMap>();
		RegisterTransformMapType<FTransformUniformScaleTranslateMap>();
		RegisterTransformMapType<FTransformNonlinearFrustumMap>();
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

	bool GetFileMetadata(TArray<FVoxelFileMetadata> &OutFileMetadata) const
	{
		bool isAnyFileMetadataValid = false;
		for (auto i = DatabaseFileMetadata.beginMeta(); i != DatabaseFileMetadata.endMeta(); ++i)
		{
			const FString metaNameStr = FROM_GRID_DATABASE_STRING(i->first);
			const FString metaTypeStr = FROM_GRID_DATABASE_STRING(i->second->typeName());
            OutFileMetadata.Add(FVoxelFileMetadata(metaNameStr, metaTypeStr));
		}
        isAnyFileMetadataValid = DatabaseFileMetadata.metaCount() > 0;
		return isAnyFileMetadataValid;
	}

	template<typename ValueType>
	bool GetFileMetadataValue(const FString &MetaName, ValueType &OutValue, FVoxelFileMetadata &OutAttributes) const
	{
		bool isMetadataValueFound = false;
		FMetadata<ValueType>::FPtr metaPtr = DatabaseFileMetadata.getMetadata<ValueType>(TO_GRID_DATABASE_STRING(MetaName));
		const FGridDatabaseString typeNameStdStr = metaPtr ? metaPtr->typeName() : FMetadataUnknown().typeName();
        FVoxelFileMetadata MetaAttributes(MetaName, FROM_GRID_DATABASE_STRING(typeNameStdStr));
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
        FVoxelFileMetadata MetaAttributes;
		return GetFileMetadataValue<ValueType>(MetaName, OutValue, MetaAttributes);
	}

	/* Return the database metadata */
	bool GetDatabaseMetadata(FVoxelDatabaseMetadata& OutVoxelDatabaseMetadata) const
	{
		if (GridArchiveIsOpen)
		{
			OutVoxelDatabaseMetadata.LibraryVersion = FGridArchive::libraryVersion();
            OutVoxelDatabaseMetadata.SupportsPartialGridReading = FGridArchive::isDelayedLoadingEnabled();
            OutVoxelDatabaseMetadata.FileCompression = (EGridCompression)FGridArchive::compression();
            OutVoxelDatabaseMetadata.UUID = FGridArchive::getUniqueTag();
		}
		return GridArchiveIsOpen;
	}

	bool OpenDatabase()
	{
		if (!GridArchiveIsOpen)
		{
			GridArchiveIsOpen = OpenGridArchive();
		}
		return GridArchiveIsOpen;
	}

protected:

	inline bool IsByteStreamInitialized() const
	{
		return BytesAccessor != nullptr;
	}

	inline bool IsByteStreamOpen() const
	{
		IsByteStreamInitialized() && FStream<boost::iostreams::stream<boost::iostreams::array>>::is_open();
	}

	inline bool OpenByteStream()
	{
		//if (BytesCount <= UINT32_MAX) {} TODO error handling
		if (!IsByteStreamInitialized())
		{
			//Preallocate memory, associate the stream buffer to the memory, then open the stream on top of the stream buffer
			Bytes.SetNumUninitialized(BytesCount, false);
			BytesAccessor.reset(new boost::iostreams::array(Bytes.GetData(), Bytes.GetData() + Bytes.Num()));
			FStream<boost::iostreams::stream<boost::iostreams::array>>::open(*BytesAccessor);
		}
		return IsByteStreamOpen();
	}

	/* If the database is not already open, reset the stream buffer to the beginning of the file and read the header and grid descriptors */
	bool OpenGridArchive()
	{
		checkue4(!GridArchiveIsOpen);

		bool isGridArchiveOpen = false;
		if (!IsByteStreamOpen() && OpenByteStream())
		{
			auto& BytesStream = static_cast<boost::iostreams::stream<boost::iostreams::array>&>(*this);

			//Read the file header as the first item in the buffer
			FGridArchive::readHeader(BytesStream);

			//openvdb::io::StreamMetadata
			//Initialize I/O stream metadata
			//StreamPtr.reset(new FStreamMetadata<std::iostream>(this)); TODO Add mechanism to reset the stream metadata
			FGridArchive::setFormatVersion(BytesStream);
			FGridArchive::setLibraryVersion(BytesStream);
			FGridArchive::setDataCompression(BytesStream);

			//DO NOT set a mapped file.
			//If a mapped file is set, leaf nodes would attempt to read in their buffers via the boost mapped file contained in the unchangeable Impl.
			//In addition to not setting the mapped file, OPENVDB_2_ABI_COMPATIBLE must be defined to avoid other mapped file operations under the covers of openvdb::io.
			//The mapped file pointer is null by default as long as openvdb::io::setMappedFilePtr isn't called.
			//For reference, if a mapped file was desired and #ifndef OPENVDB_2_ABI_COMPATIBLE, this would be the next call:
			//openvdb::io::setMappedFilePtr(StreamBuffer, mFileMapping);

			//Read the file-level metadata
            DatabaseFileMetadata.clearMetadata();
            DatabaseFileMetadata.readMeta(BytesStream);

			//Read the number of grids then for each grid read just the grid descriptor (not grid data)
			NumGrids = FGridArchive::readGridCount(BytesStream);
			GridDescriptors.clear();
			for (int32 i = 0; i < NumGrids; ++i)
			{
				FGridDescriptorNameMapCIter GridDescriptorIter = FGridDescriptor::ReadAndAddNextGridDescriptor<boost::iostreams::stream<boost::iostreams::array>>(*this, GridDescriptors);
				if (GridDescriptorIter != GridDescriptors.end())
				{
					//TODO log success
				}
			}

			isGridArchiveOpen = true;
		}
		return isGridArchiveOpen;
	}

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
	uint64 BytesCount;
	TMap<FGridName, uint64> MaxGridSizeByType;

	FString GridArchiveName;
	bool GridArchiveIsOpen;
	int32 NumGrids;

	FGridDescriptorNameMap GridDescriptors;
	FMetaMap DatabaseFileMetadata;

    TArray<FString> RegisteredGridTypeDisplayNames;
    TArray<FString> RegisteredMetadataTypeDisplayNames;
    TArray<FString> RegisteredTransformMapTypeDisplayNames;
};
