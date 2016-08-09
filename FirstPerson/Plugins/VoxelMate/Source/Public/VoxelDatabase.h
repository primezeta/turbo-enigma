#pragma once
#include "String.h"
#include "Archive.h"
#include "VoxelData.h"

#pragma warning(push)
#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <openvdb/io/Queue.h>
#include <openvdb/io/Stream.h>
#include <openvdb/tools/Prune.h>
//#include "GridOps.h"
//#include "GridMetadata.h"

#if DO_CHECK
#undef verify
#undef check
#define verifyue4(expr)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } } 
#define checkue4(expr) { if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } }
#endif

#include <fstream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string/predicate.hpp> // for boost::ends_with()
#include <boost/algorithm/string/erase.hpp> // for boost::erase_last()

#include "GridDescriptorInOut.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelDatabase, Log, All)

char* const META_MEM_BLOCK_NAME = "mem_block_name"; //TODO Make static

//5,4,3 is the standard openvdb tree configuration
typedef openvdb::tree::Tree4<FVoxelData, 5, 4, 3>::Type DefaultTreeType;
//class FVoxelGrid;
//class FVoxelMetaData;
//class FVoxelTreeData;
//class FVoxelTransform;

#pragma pack(push,VoxelDatabaseInfoPack,1)
struct FVoxelDatabaseInfo
{
	typedef boost::shared_ptr<FVoxelDatabaseInfo> Ptr;

	FVoxelDatabaseInfo()
		: MagicNumber(0), MajorVersion(0), MinorVersion(0), SupportsPartialGridReading(false), FileCompression(openvdb::io::COMPRESS_NONE)
	{
		FMemory::Memset(UUID, 0, 16);
	}

	//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
	int64 MagicNumber;
	uint32 MajorVersion;
	uint32 MinorVersion;
	bool SupportsPartialGridReading;
	char FileCompression;
	char UUID[16];
};
#pragma pack(pop,VoxelDatabaseInfoPack)

struct FGridMemoryMappingInfo
{
	openvdb::GridBase::Ptr GridPtr;
	std::string GridName;
	std::string AllocationName;
};

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

	static bool IsMetadataReadOnly(const FString &MetaName)
	{
		const std::string &metaNameStdStr = TCHAR_TO_UTF8(*MetaName);
		const bool isReadOnly =
			metaNameStdStr == openvdb::GridBase::META_GRID_CLASS		||
			metaNameStdStr == openvdb::GridBase::META_GRID_CREATOR		||
			metaNameStdStr == openvdb::GridBase::META_GRID_NAME			||
			metaNameStdStr == openvdb::GridBase::META_SAVE_HALF_FLOAT	||
			metaNameStdStr == openvdb::GridBase::META_IS_LOCAL_SPACE	||
			metaNameStdStr == openvdb::GridBase::META_VECTOR_TYPE		||
			metaNameStdStr == openvdb::GridBase::META_FILE_BBOX_MIN		||
			metaNameStdStr == openvdb::GridBase::META_FILE_BBOX_MAX		||
			metaNameStdStr == openvdb::GridBase::META_FILE_COMPRESSION	||
			metaNameStdStr == openvdb::GridBase::META_FILE_MEM_BYTES;
		return isReadOnly;
	}
};

template<typename ValueType> static inline void
ConstructUniqueSharedMemoryValue(const std::string &MemoryNamePrefix, ValueType *OutPtr, std::string &OutMemoryName, boost::interprocess::managed_shared_memory &OutSharedMemory)
{
	//Index common to each ValueType instantiation, used to create a unique name when names clash. TODO: Better method to ensure unique names?
	static int32 ValueTypeIndex = 0;

	//Construct a unique name for the memory segment
	std::pair<ValueType*, std::size_t> memorySegment(nullptr, 0);
	do
	{
		std::ostringstream ss;
		ss << MemoryNamePrefix << "_" << ValueTypeIndex++;
		OutMemoryName = ss.str();
		memorySegment = OutSharedMemory.find<ValueType>(OutMemoryName.c_str());
	} while (memorySegment.first != nullptr);

	//Construct the value in shared memory and return the address encapsulated in a shared pointer
	auto rawPtr = OutSharedMemory.construct<ValueType>(OutMemoryName.c_str());
	OutPtr = reinterpret_cast<ValueType*>(rawPtr());
}

template<typename ValueType> static inline void
ConstructUniqueSharedMemoryValue(const std::string &MemoryNamePrefix, boost::shared_ptr<ValueType> &OutPtr, std::string &OutMemoryName, boost::interprocess::managed_shared_memory &OutSharedMemory)
{
	ValueType *RawPtr = nullptr;
	ConstructUniqueSharedMemoryValue(MemoryNamePrefix, RawPtr, OutMemoryName, OutSharedMemory);
	OutPtr.reset(RawPtr);
}

template<typename ValueType> static inline void
ConstructUniqueSharedMemoryValue(const std::string &MemoryNamePrefix, TSharedPtr<ValueType> &OutPtr, std::string &OutMemoryName, boost::interprocess::managed_shared_memory &OutSharedMemory)
{
	ValueType *RawPtr = nullptr;
	ConstructUniqueSharedMemoryValue(MemoryNamePrefix, RawPtr, OutMemoryName, OutSharedMemory);
	OutPtr = RawPtr;
}

//FGridFactory only to be used to gain access to GridBase::registerGrid
template<typename TreeType>
class FGrid : public openvdb::Grid<TreeType>
{
public:
	typedef boost::shared_ptr<FGrid> Ptr;

	// Register a grid type along with a factory function.
	static void RegisterGrid(const openvdb::Name& gridType, TFunction<openvdb::GridBase::Ptr(void)> factoryFunc)
	{
		try
		{
			openvdb::GridBase::registerGrid(gridType(), factoryFunc.GetCallable());
		}
		catch (openvdb::KeyError &e)
		{
			//Grid type is already registered - nothing to do
		}
	}

	static Ptr CreateGrid(boost::interprocess::managed_shared_memory &SharedMemory)
	{
		const std::string baseMemName = gridType();
		std::string uniqueMemName;
		Ptr gridPtr;
		ConstructUniqueSharedMemoryValue<FGrid>(baseMemName, gridPtr, uniqueMemName, SharedMemory);
		checkue4(gridPtr != nullptr);
		gridPtr->insertMeta(META_MEM_BLOCK_NAME, uniqueMemName);		
		return gridPtr;
	}
};

class FVoxelDatabase : public FArchive, public openvdb::io::Archive
{
public:
	FVoxelDatabase(const FString &name)
		: GridArchiveName(name),
		SharedMemory(boost::interprocess::open_or_create, TCHAR_TO_UTF8(*name), sizeof(FVoxelDatabaseInfo)),
		MemoryBuffer(static_cast<char*>(SharedMemory.get_address()), SharedMemory.get_size()),
		StreamBuffer(MemoryBuffer)
	{
		openvdb::initialize();
		GridArchiveIsOpen = false;
		GridDescriptors.clear();
		StreamMetadataPtr = nullptr;

		//Initially allocate enough memory to read in the vdb file header
		//NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
		//Header size is int64:magic number, 2*uint32:file version, char:has offsets flag, 16*char:128-bit boost::uuids::uuid
		const boost::ulong_long_type FileHeaderSizeTest = sizeof(int64) + (2 * sizeof(uint32)) + sizeof(char) + (16 * sizeof(char));
		const boost::ulong_long_type FileHeaderSize = sizeof(FVoxelDatabaseInfo);
		checkue4(FileHeaderSizeTest == FileHeaderSize);
		//SharedMemory.allocate(FileHeaderSize); TODO: Need to allocate after open_or_create?
		openvdb::io::setCurrentVersion(StreamBuffer);
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal, and Leaf
	*  Dimensions: Internal = N1, Leaf = N2
	*/
	template<typename DataType, typename N1, typename N2>
	void RegisterGrid3()
	{
		typedef openvdb::Tree3<DataType, N1, N2> Tree3Type;
		RegisterGridType<Tree3Type>();
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, and Leaf
	*  Dimensions: Internal0 = N1, Internal1 = N2, Leaf = N3
	*/
	template<typename DataType, typename N1, typename N2, typename N3>
	void RegisterGrid4()
	{
		typedef openvdb::Tree4<DataType, N1, N2, N3> Tree4Type;
		RegisterGridType<Tree4Type>();
	}

	/* Register a grid containing voxels of DataType with tree topology Root, Internal0, Internal1, Internal2, and Leaf
	*  Dimensions: Internal0 = N1, Internal1 = N2, Internal2 = N3, Leaf = N4
	*/
	template<typename DataType, typename N1, typename N2, typename N3, typename N4>
	void RegisterGrid5()
	{
		typedef openvdb::Tree5<DataType, N1, N2, N3, N4> Tree5Type;
		RegisterGridType<Tree5Type>();
	}

	/* Register a grid containing voxels of DataType with the standard tree topology of Internal0 = 5, Internal1 = 4, Leaf = 3
	*/
	template<typename DataType>
	void RegisterStandardGrid()
	{
		RegisterGrid4<DataType, 5, 4, 3>();
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
		if (DatabaseMetadata != nullptr)
		{
			auto &db = *DatabaseMetadata;
			for (auto i = db.beginMeta(); i != db.endMeta(); ++i)
			{
				const FString metaNameStr = UTF8_TO_TCHAR(i->first.c_str());
				const FString metaTypeStr = UTF8_TO_TCHAR(i->second->typeName().c_str());
				OutAttributes.Add(FVoxelDatabaseMetadataAttributes(metaNameStr, metaTypeStr));
			}
		}
		return areAnyAttributesValid;
	}

	template<typename ValueType>
	bool GetFileMetadataValue(const FString &MetaName, ValueType &OutValue, FVoxelDatabaseMetadataAttributes &OutAttributes) const
	{
		bool isFileMetadataFound = false;
		if (DatabaseMetadata != nullptr)
		{
			auto &db = *DatabaseMetadata;
			openvdb::TypedMetadata<ValueType>::Ptr metaPtr = db.getMetadata<ValueType>(TCHAR_TO_UTF8(*MetaName));
			const std::string typeNameStdStr = metaPtr ? metaPtr->typeName().c_str() : openvdb::UnknownMetadata().typeName();
			FVoxelDatabaseMetadataAttributes MetaAttributes(MetaName, UTF8_TO_TCHAR(typeNameStdStr.c_str()));
			if (metaPtr)
			{
				OutValue = *metaPtr;
				OutAttributes = MetaAttributes;
				isFileMetadataFound = true;
			}
			UE_LOG(LogVoxelDatabase, Verbose, TEXT("%s"), MetaAttributes.ToString());
		}
		return isFileMetadataFound;
	}

	template<typename ValueType>
	bool GetFileMetadataValue(const FString &MetaName, ValueType &OutValue) const
	{
		FVoxelDatabaseMetadataAttributes MetaAttributes;
		return GetFileMetadataValue<ValueType>(MetaName, OutValue, MetaAttributes);
	}

	/* Reset the stream buffer to the beginning of the file and read the header and grid descriptors */
	bool OpenDatabase()
	{
		bool isOpen = false;

		//Reset the stream buffer in case it had been open previously TODO: Handle shared memory if the database is opened twice
		StreamBuffer.clear();

		//Read the file header (memory for which was allocated in the constructor)
		Archive::readHeader(StreamBuffer);
		DatabaseFileHeader = FVoxelDatabaseInfo::Ptr(reinterpret_cast<FVoxelDatabaseInfo*>(SharedMemory.get_address()));

		//Initialize I/O stream metadata
		const bool transfer = false;
		StreamMetadataPtr.reset(new openvdb::io::StreamMetadata());
		openvdb::io::setStreamMetadataPtr(StreamBuffer, StreamMetadataPtr, transfer);
		Archive::setFormatVersion(StreamBuffer);
		Archive::setLibraryVersion(StreamBuffer);
		Archive::setDataCompression(StreamBuffer);

		//We do not want to set a mapped file the leaf nodes would attempt to read in their buffers via the boost mapped file contained in the unchangeable Impl.
		//For purposes of reading the grid from the archive, we want OPENVDB_2_ABI_COMPATIBLE to be NOT defined and the mapped file pointer in the stream meta to be null.
		//The mapped file pointer is null by default as long as openvdb::io::setMappedFilePtr isn't called.
		//This would be the next call if we were reading directly from file and #ifndef OPENVDB_2_ABI_COMPATIBLE:
		//openvdb::io::setMappedFilePtr(StreamBuffer, mFileMapping);

		const std::string gridArchiveNameStdStr = TCHAR_TO_UTF8(*GridArchiveName);

		//Allocate memory for the metamap and then read the metamap into the new memory
		const std::string fileMetaDataName = gridArchiveNameStdStr + "_metaData";
		auto metaRawPtr = SharedMemory.construct<openvdb::MetaMap>(fileMetaDataName.c_str());
		DatabaseMetadata = openvdb::MetaMap::Ptr(metaRawPtr());
		DatabaseMetadata->readMeta(StreamBuffer);

		//Allocate memory for the number of grids and then read the number of grids into the new memory
		const std::string numGridsName = gridArchiveNameStdStr + "_numGrids";
		auto numGridsRawPtr = SharedMemory.construct<int32>(numGridsName.c_str());
		const int32 numGrids = readGridCount(StreamBuffer);

		//For each grid allocate memory for the grid descriptor, read in the grid descriptor, and then seek past the grid data to the next grid descriptor
		GridDescriptors.clear();
		const std::string gridDescriptorNamePrefix = gridArchiveNameStdStr + "_gridDescriptor";
		for (int32 i = 0; i < numGrids; ++i)
		{
			//Read the unique name (which is the first grid descriptor value) singly so that we can use it for the memory block name
			std::string gridDescriptorUniqueName = "";
			FGridDescriptor::ReadString(StreamBuffer, gridDescriptorUniqueName);

			//Allocate and create a default grid descriptor
			std::string AllocationName;
			FGridDescriptor::Ptr GridDescriptorPtr;
			ConstructUniqueSharedMemoryValue(gridDescriptorUniqueName, GridDescriptorPtr, AllocationName, SharedMemory);

			//Read the grid descriptor from the archive
			const bool isGridTypeRegistered = FGridDescriptor::ReadGridDescriptor(StreamBuffer, gridDescriptorUniqueName, *GridDescriptorPtr);
			GridDescriptorPtr->AllocationName = AllocationName; //Allocation name is not read from file
			GridDescriptors.insert(std::pair<const openvdb::Name, FGridDescriptor::Ptr>(gridDescriptorUniqueName, GridDescriptorPtr));

			// Create the grid only if the grid type is registered
			openvdb::GridBase::Ptr GridPtr = nullptr;
			if (isGridTypeRegistered)
			{
				//Create the grid via the base class, from which a call to the registered factory is made by the grid registry
				GridPtr = openvdb::GridBase::createGrid(GridDescriptorPtr->GridType);
				if (GridPtr != nullptr)
				{
					//Set metadata values for float-as-half and grid name
					GridPtr->setSaveFloatAsHalf(GridDescriptorPtr->IsFloatSavedAsHalf);
					GridPtr->setName(GridDescriptorPtr->GridName);
					//TODO Not sure yet if the following will be set here
					//GridPtr->setCreator()
					//GridPtr->setGridClass()
					//GridPtr->setIsInWorldSpace()
					//GridPtr->setVectorType()
					UE_LOG(LogVoxelDatabase, Display, TEXT("Created %s grid %s"),
						UTF8_TO_TCHAR(GridDescriptorPtr->GridType.c_str()),
						UTF8_TO_TCHAR(GridDescriptorPtr->GridName.c_str()));
				}
				else
				{
					UE_LOG(LogVoxelDatabase, Error, TEXT("Failed to create %s grid %s"),
						UTF8_TO_TCHAR(GridDescriptorPtr->GridType.c_str()),
						UTF8_TO_TCHAR(GridDescriptorPtr->GridName.c_str()));
				}
			}
			else
			{
				//The grid could not be read because it wasn't registered.
				//Log the unreadable grid and continue as normal.
				UE_LOG(LogVoxelDatabase, Error, TEXT("Could not create grid %s because grid type %s is not registered"),
					UTF8_TO_TCHAR(GridDescriptorPtr->GridName.c_str()),
					UTF8_TO_TCHAR(GridDescriptorPtr->GridType.c_str()));
			}

			//Skip forward to the next descriptor
			GridDescriptorPtr->SeekToEnd(StreamBuffer);
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
	template<typename TreeType>
	void RegisterGridType()
	{
		if (!FGrid<TreeType>::isRegistered())
		{
			//Registering a grid has an effect such that the following TFunction will be called when GridBase::createGrid is called.
			TFunction<openvdb::GridBase::Ptr(void)> CreateGridFunctor([&]() {
				return FGrid<TreeType>::CreateGrid(GridDescriptors, SharedMemory);
			});
			FGrid<TreeType>::RegisterGrid(FGrid<TreeType>::gridType(), CreateGridFunctor);
		}
	}

	openvdb::io::GridDescriptorNameMapCIter FindGridDescriptor(const FString &gridName) const
	{
		const openvdb::Name name = openvdb::Name(TCHAR_TO_UTF8(*gridName));
		const openvdb::Name uniqueName = openvdb::io::GridDescriptor::stringAsUniqueName(name);

		// Find all descriptors with the given grid name.
		std::pair<openvdb::io::GridDescriptorNameMapCIter, openvdb::io::GridDescriptorNameMapCIter> range = GridDescriptors.equal_range(name);

		if (range.first == range.second)
		{
			// If no descriptors were found with the given grid name, the name might have
			// a suffix ("name[N]").  In that case, remove the "[N]" suffix and search again.
			range = GridDescriptors.equal_range(openvdb::io::GridDescriptor::stripSuffix(uniqueName));
		}

		const size_t count = size_t(std::distance(range.first, range.second));
		if (count > 1 && name == uniqueName)
		{
			UE_LOG(LogVoxelDatabase, Error, TEXT("%s has more than one grid named %s"), *GetArchiveName(), *gridName);
		}

		openvdb::io::GridDescriptorNameMapCIter foundGridDescriptor = GridDescriptors.end();
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
				for (openvdb::io::GridDescriptorNameMapCIter i = range.first; i != range.second; ++i)
				{
					const openvdb::Name candidateName = i->second->UniqueName;
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
	boost::interprocess::managed_shared_memory SharedMemory;
	boost::iostreams::stream<boost::iostreams::array> StreamBuffer;
	boost::iostreams::array MemoryBuffer;

	const FString GridArchiveName;
	bool GridArchiveIsOpen;
	uint32 FileVersion;
	openvdb::io::GridDescriptorNameMap GridDescriptors;
	FVoxelDatabaseInfo::Ptr DatabaseFileHeader;
	openvdb::MetaMap::Ptr DatabaseMetadata;
	openvdb::io::StreamMetadata::Ptr StreamMetadataPtr;
};