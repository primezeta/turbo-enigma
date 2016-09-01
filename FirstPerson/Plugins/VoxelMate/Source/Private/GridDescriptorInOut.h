#pragma once
#include "Platform.h"
#include "Engine.h" //TODO: Engine.h is included just for UENUM, UCLASS, etc. Find exact header for these defines

#pragma warning(push)
#pragma warning(1:4211 4800 4503 4146)
#include <openvdb/openvdb.h>
#include <openvdb/math/Maps.h>
#include <openvdb/io/io.h>
#include <openvdb/util/Name.h>
#include <openvdb/io/Stream.h>
#include <openvdb/tools/Prune.h>
#include <openvdb/tools/PointIndexGrid.h>
//#include "GridOps.h"
//#include "GridMetadata.h"
#include "VoxelMateStreams.h"

//TODO Logging for FGridDescriptor
//DECLARE_LOG_CATEGORY_EXTERN(LogGridDescriptor, Log, All)

#define TO_GRID_DATABASE_STRING(fstring) TCHAR_TO_UTF8(*fstring)
#define FROM_GRID_DATABASE_STRING(grid_database_string) UTF8_TO_TCHAR(grid_database_string.c_str())

//The half-float typename suffix is declared in GridDescriptor.cc
extern const char* HALF_FLOAT_TYPENAME_SUFFIX; //TODO Move this to it's own .h file

class FGridDescriptor;

typedef openvdb::GridBase FGridBase;
typedef std::string FGridDatabaseString;
typedef openvdb::Name FGridName;
typedef openvdb::Index FIndex;
typedef openvdb::VersionId FVersionId;
typedef std::multimap<FGridName, FGridDescriptor> FGridDescriptorNameMap;
typedef FGridDescriptorNameMap::iterator FGridDescriptorNameMapIter;
typedef FGridDescriptorNameMap::const_iterator FGridDescriptorNameMapCIter;
typedef openvdb::MetaMap FMetaMap;
typedef openvdb::PointIndex32 FPointIndex32;

//openvdb::io::COMPRESS_NONE
//openvdb::io::COMPRESS_ZIP
//openvdb::io::COMPRESS_ACTIVE_MASK
//openvdb::io::COMPRESS_BLOSC
//NOTE: Must ensure these types remain aligned. Unfortunately must set UEnum values to literal
UENUM(BlueprintType)
enum class EGridCompression : uint8
{
	GridCompressionNone       = 0x0,
	GridCompressionZip        = 0x1,
	GridCompressionActiveMask = 0x2,
	GridCompressionBlosc      = 0x4
};

//openvdb::GRID_UNKNOWN
//openvdb::GRID_LEVEL_SET
//openvdb::GRID_FOG_VOLUME
//openvdb::GRID_STAGGERED
//NOTE: Must ensure these types remain aligned. Unfortunately must set UEnum values to literal
UENUM(BlueprintType)
enum class EGridClass : uint8
{
	GridClassUnknown = 0,
	GridClassLevelSet,
	GridClassFogVolume,
	GridClassStaggered
};

namespace GridExceptions
{
	typedef openvdb::ArithmeticError FArithmeticError;
	typedef openvdb::IllegalValueException FIllegalValueError;
	typedef openvdb::IndexError FIndexError;
	typedef openvdb::IoError FIoError;
	typedef openvdb::KeyError FKeyError;
	typedef openvdb::LookupError FLookupError;
	typedef openvdb::NotImplementedError FNotImplementedError;
	typedef openvdb::ReferenceError FReferenceError;
	typedef openvdb::RuntimeError FRuntimeError;
	typedef openvdb::TypeError FTypeError;
	typedef openvdb::ValueError FValueError;
};

class FStreamMetadata : public openvdb::io::StreamMetadata
{
public:
	typedef boost::shared_ptr<FStreamMetadata> FPtr;
};

namespace GridIOStatics
{
	const static uint32 FileVersionGridInstancing = openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING;
	//std::string getErrorString(int errorNum); TODO
	//std::string getErrorString(); TODO
	static inline uint32 GetFormatVersion(std::ios_base& iosb)
	{
		return openvdb::io::getFormatVersion(iosb);
	}

	static inline FVersionId GetLibraryVersion(std::ios_base& iosb)
	{
		return openvdb::io::getLibraryVersion(iosb);
	}

	static inline FGridDatabaseString GetVersion(std::ios_base& iosb)
	{
		return openvdb::io::getVersion(iosb);
	}

	static inline void SetCurrentVersion(std::istream& is)
	{
		openvdb::io::setCurrentVersion(is);
	}

	static inline void SetVersion(std::ios_base& iosb, const FVersionId& libraryVersion, uint32 fileVersion)
	{
		openvdb::io::setVersion(iosb, libraryVersion, fileVersion);
	}

	static inline uint32 GetDataCompression(std::ios_base& iosb)
	{
		return openvdb::io::getDataCompression(iosb);
	}

	static inline void SetDataCompression(std::ios_base& iosb, uint32 compressionFlags)
	{
		openvdb::io::setDataCompression(iosb, compressionFlags);
	}

	static inline EGridClass GetGridClass(std::ios_base& iosb)
	{
		return (EGridClass)openvdb::io::getGridClass(iosb);
	}

	static inline void SetGridClass(std::ios_base& iosb, EGridClass gridClass)
	{
		openvdb::io::setGridClass(iosb, (uint32)gridClass);
	}

	static inline bool GetHalfFloat(std::ios_base& iosb)
	{
		return openvdb::io::getHalfFloat(iosb);
	}

	static inline void SetHalfFloat(std::ios_base& iosb, bool isHalfFloat)
	{
		openvdb::io::setHalfFloat(iosb, isHalfFloat);
	}

	static inline const void* GetGridBackgroundValuePtr(std::ios_base& iosb)
	{
		return openvdb::io::getGridBackgroundValuePtr(iosb);
	}

	static inline void SetGridBackgroundValuePtr(std::ios_base& iosb, const void* backgroundValuePtr)
	{
		openvdb::io::setGridBackgroundValuePtr(iosb, backgroundValuePtr);
	}

	static inline bool GetWriteGridStatsMetadata(std::ios_base& iosb)
	{
		return openvdb::io::getWriteGridStatsMetadata(iosb);
	}

	static inline void SetWriteGridStatsMetadata(std::ios_base& iosb, bool isGridStatsWritten)
	{
		openvdb::io::setWriteGridStatsMetadata(iosb, isGridStatsWritten);
	}

	//Do not need openvdb mapped files
	//static inline boost::shared_ptr<openvdb::io::MappedFile> GetMappedFilePtr(std::ios_base& iosb)
	//{
	//	return openvdb::io::getMappedFilePtr(iosb);
	//}

	//Do not need openvdb mapped files
	//static inline void SetMappedFilePtr(std::ios_base& iosb, boost::shared_ptr<MappedFile>& mappedFilePtr)
	//{
	//	openvdb::io::setMappedFilePtr(iosb, mappedFilePtr);
	//}

	static inline openvdb::io::StreamMetadata::Ptr GetStreamMetadataPtr(std::ios_base& iosb)
	{
		return openvdb::io::getStreamMetadataPtr(iosb);
	}

	static inline void SetStreamMetadataPtr(std::ios_base& iosb, FStreamMetadata::Ptr streamMetadataPtr, bool isStreamTransferred)
	{
		openvdb::io::StreamMetadata::Ptr streamMetadataBasicPtr = boost::static_pointer_cast<openvdb::io::StreamMetadata>(streamMetadataPtr);
		openvdb::io::setStreamMetadataPtr(iosb, streamMetadataBasicPtr, isStreamTransferred);
	}

	static inline openvdb::io::StreamMetadata::Ptr ClearStreamMetadataPtr(std::ios_base& iosb)
	{
		return openvdb::io::clearStreamMetadataPtr(iosb);
	}
};

namespace MetadataStatics
{
	static inline void ClearRegistry()
	{
		openvdb::Metadata::clearRegistry();
	}
};

namespace TransformMapStatics
{
	static inline void ClearRegistry()
	{
		openvdb::math::MapRegistry::clear();
	}
}

class FGridArchive : public openvdb::io::Archive
{
public:
	typedef boost::shared_ptr<FGridArchive> FPtr;
	typedef const FPtr FConstPtr;
};

template<typename DataType>
using FTree3 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>>>;

template<typename DataType>
using FTree4 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>, 5>>>;

template<typename DataType>
using FTree5 = openvdb::tree::Tree<openvdb::tree::RootNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::InternalNode<openvdb::tree::LeafNode<DataType, 3>, 4>, 5>, 6>>>;

struct GridBaseStatics : public openvdb::GridBase
{
	typedef FGridBase::Ptr FPtr;
	typedef FGridBase::ConstPtr FConstPtr;

	const static FGridDatabaseString MetaNameGridClass;
	const static FGridDatabaseString MetaNameGridCreator;
	const static FGridDatabaseString MetaNameGridName;
	const static FGridDatabaseString MetaNameSaveHalfFloat;
	const static FGridDatabaseString MetaNameIsLocalSpace;
	const static FGridDatabaseString MetaNameVectorType;
	const static FGridDatabaseString MetaNameFileBBoxMin;
	const static FGridDatabaseString MetaNameFileBBoxMax;
	const static FGridDatabaseString MetaNameFileCompression;
	const static FGridDatabaseString MetaNameFileMemBytes;

	static inline void ClearRegistry()
	{
		FGridBase::clearRegistry();
	}

	static inline FPtr CreateGrid(const FGridName& type)
	{
		return FGridBase::createGrid(type);
	}

	static inline bool IsRegistered(const FGridName& type)
	{
		return FGridBase::isRegistered(type);
	}

	/// Register a grid type along with a factory function.
	//static void registerGrid(const Name& type, GridFactory);
	/// Remove a grid type from the registry.
	//static void unregisterGrid(const Name& type);

	static inline void RegisterGrid(const FGridName& type, openvdb::GridBase::GridFactory factory)
	{
		openvdb::GridBase::registerGrid(type, factory);
	}

	static inline void UnregisterGrid(const FGridName& type)
	{
		openvdb::GridBase::unregisterGrid(type);
	}
};

template<typename TreeType>
class FGrid : public openvdb::Grid<TreeType>
{
public:
	typedef typename boost::shared_ptr<FGrid<TreeType>> FPtr;
	typedef const FPtr FConstPtr;
	typedef typename openvdb::Grid<TreeType>::TreeType FTreeType;
	typedef openvdb::Grid<TreeType>::Accessor FAccessor;
	typedef openvdb::Grid<TreeType>::ConstAccessor FConstAccessor;
	typedef openvdb::Grid<TreeType>::UnsafeAccessor FUnsafeAccessor;
	typedef openvdb::Grid<TreeType>::ConstUnsafeAccessor FConstUnsafeAccessor;

	// Register a grid type name and the construction functor to call when a grid associated to the type name is unserialized
	template<typename TreeType>
	static void RegisterGrid()
	{
		try
		{
			GridBaseStatics::RegisterGrid(openvdb::Grid<TreeType>::gridType(), CreateGrid<TreeType>);
		}
		catch (GridExceptions::FKeyError &e)
		{
			//Grid type is already registered - nothing to do
			//TODO logging and error handling
			std::string x = e.what();
		}
	}

	template<typename TreeType>
	static GridBaseStatics::FPtr CreateGrid()
	{
		GridBaseStatics::FPtr GridPtr = nullptr;
		try
		{
			GridPtr = openvdb::Grid<TreeType>::create();
		}
		catch (GridExceptions::FKeyError &e)
		{
			//TODO logging and error handling
			std::string x = e.what();
		}
		check(GridPtr != nullptr);
		return GridPtr;
	}

	template<typename TreeType>
	static bool IsRegistered()
	{
		return FGrid<TreeType>::isRegistered();
	}

	static GridBaseStatics::FPtr ReadGrid(std::istream& is, FGridDescriptorNameMap& GridDescriptors)
	{
		FGridDescriptor& GridDescriptor = FGridDescriptor::ReadAndInsertGridDescriptor(is, GridDescriptors);
		GridBaseStatics::FPtr GridPtr = GridDescriptor.GridPtr;
		return GridPtr;
	}
};

class FMetadataUnknown : public openvdb::UnknownMetadata
{
public:
	typedef openvdb::UnknownMetadata::Ptr FPtr;
};

template<typename MetaType>
class FMetadata : public openvdb::TypedMetadata<MetaType>
{
public:
	typedef boost::shared_ptr<FMetadata<MetaType>> FPtr;

	// Register a metadata type name and the construction functor to call when a metadata item associated to the type name is unserialized
	template<typename MetaType>
	static void RegisterMeta()
	{
		try
		{
			openvdb::Metadata::registerType(FMetadata<MetaType>::staticTypeName(), CreateMeta<MetaType>);
		}
		catch (GridExceptions::FKeyError &e)
		{
			//Meta type is already registered - nothing to do
			//TODO logging and error handling
			std::string x = e.what();
		}
	}

	template<typename MetaType>
	static openvdb::Metadata::Ptr CreateMeta()
	{
		openvdb::Metadata::Ptr MetaPtr = nullptr;
		try
		{
			MetaPtr = openvdb::TypedMetadata<MetaType>::createMetadata();
		}
		catch (GridExceptions::FKeyError &e)
		{
			//TODO logging and error handling
			std::string x = e.what();
		}
		check(MetaPtr != nullptr);
		return MetaPtr;
	}

	template<typename MetaType>
	static bool IsRegisteredType()
	{
		return FMetadata<MetaType>::isRegisteredType();
	}
};

template<typename MapType>
class FTransformMap : public MapType
{
public:
	typedef boost::shared_ptr<FTransformMap<MapType>> FPtr;

	// Register a transform map type name and the construction functor to call when a transform map item associated to the type name is unserialized
	template<typename MapType>
	static void RegisterTransformMap()
	{
		try
		{
			openvdb::math::MapRegistry::registerMap(MapType::mapType(), CreateTransformMap<MapType>);
		}
		catch (GridExceptions::FKeyError &e)
		{
			//Transform map type is already registered - nothing to do
			//TODO logging and error handling
			std::string x = e.what();
		}
	}

	template<typename MapType>
	static openvdb::math::MapBase::Ptr CreateTransformMap()
	{
		openvdb::math::MapBase::Ptr MapPtr = nullptr;
		try
		{
			MapPtr = MapType::create();
		}
		catch(GridExceptions::FKeyError &e)
		{
			//TODO logging and error handling
			std::string x = e.what();
		}
		check(MapPtr != nullptr);
		return MapPtr;
	}

	template<typename MapType>
	static bool IsRegistered()
	{
		return MapType::isRegistered();
	}
};

class FGridDescriptor
{
public:
	typedef boost::shared_ptr<FGridDescriptor> FPtr;

	FGridDatabaseString GridName; //Grid name is not archived, rather it is the unique name with the suffix stripped
	FGridDatabaseString UniqueName;
	FGridDatabaseString GridType;
	FGridDatabaseString ParentGridName;
	bool IsFloatSavedAsHalf;
	int64 GridStreamPosition;
	int64 DataBlocksStreamPosition;
	int64 GridEndPosition;
	GridBaseStatics::FPtr GridPtr; //Important! When serializing a grid descriptor, do not serialize the grid

	static inline FGridDescriptor& ReadAndInsertGridDescriptor(std::istream& is, FGridDescriptorNameMap& GridDescriptors)
	{
		//Unique name is read alone previously to ReadGridDescriptor		
		FGridName uniqueName = FGridDatabaseString(ReadString(is));
		FGridDescriptorNameMapIter GridDescriptorIter = GridDescriptors.emplace(uniqueName, FGridDescriptor());
		check(GridDescriptorIter != GridDescriptors.end());

		FGridDescriptor& OutGridDescriptor = GridDescriptorIter->second;
		OutGridDescriptor.GridName = FGridDescriptor::StripSuffix(uniqueName);
		OutGridDescriptor.GridType = ReadString(is);
		OutGridDescriptor.IsFloatSavedAsHalf = boost::ends_with(OutGridDescriptor.GridType, HALF_FLOAT_TYPENAME_SUFFIX);
		
		if (OutGridDescriptor.IsFloatSavedAsHalf)
		{
			boost::erase_last(OutGridDescriptor.GridType, HALF_FLOAT_TYPENAME_SUFFIX);
		}

		const uint32 formatVersion = GridIOStatics::GetFormatVersion(is);
		if (formatVersion >= GridIOStatics::FileVersionGridInstancing)
		{
			OutGridDescriptor.ParentGridName = ReadString(is);
		}

		// Read in the offsets.
		ReadBytes<int64>(is, OutGridDescriptor.GridStreamPosition);
		ReadBytes<int64>(is, OutGridDescriptor.DataBlocksStreamPosition);
		ReadBytes<int64>(is, OutGridDescriptor.GridEndPosition);

		// Create the grid only if the grid type is registered
		const bool isGridTypeRegistered = FGridBase::isRegistered(OutGridDescriptor.GridType);
		if (isGridTypeRegistered)
		{
			//Create the grid via the base class, from which a call to the registered factory is made by the grid registry
			OutGridDescriptor.GridPtr = FGridBase::createGrid(OutGridDescriptor.GridType);
			if (OutGridDescriptor.GridPtr != nullptr)
			{
				//Set metadata values for float-as-half TODO: Figure out exactly why openvdb sets this metavalue at this point (see openvdb::io::File line 828)
				OutGridDescriptor.GridPtr->setSaveFloatAsHalf(OutGridDescriptor.IsFloatSavedAsHalf);
				//TODO Not sure yet if the following will be set here
				//GridPtr->setCreator()
				//GridPtr->setGridClass()
				//GridPtr->setIsInWorldSpace()
				//GridPtr->setVectorType()
				//TODO Logging for FGridDescriptor
				//UE_LOG(LogGridDescriptor, Display, TEXT("Created %s grid %s"),
					//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridType),
					//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridName));
			}
			else
			{
				//TODO Logging for FGridDescriptor
				//UE_LOG(LogGridDescriptor, Error, TEXT("Failed to create %s grid %s"),
					//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridType),
					//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridName));
			}
		}
		else
		{
			//The grid could not be read because it wasn't registered.
			//Log the unreadable grid and continue as normal.
			//TODO Logging for FGridDescriptor
			//UE_LOG(LogGridDescriptor, Error, TEXT("Could not create grid %s because grid type %s is not registered"),
				//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridName),
				//FROM_GRID_DATABASE_STRING(OutGridDescriptor.GridType));
		}

		//Skip forward to the next descriptor
		OutGridDescriptor.SeekToEnd(is);
		return OutGridDescriptor;
	}

	static inline FGridName StringAsUniqueName(const FGridName &name)
	{
		return openvdb::io::GridDescriptor::stringAsUniqueName(name);
	}

	static inline FGridName StripSuffix(const FGridName &name)
	{
		return openvdb::io::GridDescriptor::stripSuffix(name);
	}

	void WriteGridHeader(std::ostream &os) const
	{
		WriteString(os, UniqueName);
		if (IsFloatSavedAsHalf)
		{
			WriteString(os, GridType + HALF_FLOAT_TYPENAME_SUFFIX);
		}
		else
		{
			WriteString(os, GridType);
		}
	}

	void WriteStreamPos(std::ostream &os) const
	{
		os.write(reinterpret_cast<const char*>(&GridStreamPosition), sizeof(int64));
		os.write(reinterpret_cast<const char*>(&DataBlocksStreamPosition), sizeof(int64));
		os.write(reinterpret_cast<const char*>(&GridEndPosition), sizeof(int64));
	}

	bool IsGridInstance() const
	{
		return !ParentGridName.empty();
	}

	inline void SeekToGrid(std::istream& is) const
	{
		is.seekg(GridStreamPosition, std::ios_base::beg);
	}

	inline void SeekToBlocks(std::istream& is) const
	{
		is.seekg(DataBlocksStreamPosition, std::ios_base::beg);
	}

	inline void SeekToEnd(std::istream& is) const
	{
		is.seekg(GridEndPosition, std::ios_base::beg);
	}

	inline void SeekToGrid(std::ostream& os) const
	{
		os.seekp(GridStreamPosition, std::ios_base::beg);
	}

	inline void SeekToBlocks(std::ostream& os) const
	{
		os.seekp(DataBlocksStreamPosition, std::ios_base::beg);
	}

	inline void SeekToEnd(std::ostream& os) const
	{
		os.seekp(GridEndPosition, std::ios_base::beg);
	}

	//TODO: Determine if iostreams really need both seekg and seekp called
	inline void SeekToGrid(std::iostream& ios) const
	{
		ios.seekg(GridStreamPosition, std::ios_base::beg);
		ios.seekp(GridStreamPosition, std::ios_base::beg);
	}

	inline void SeekToBlocks(std::iostream& ios) const
	{
		ios.seekg(DataBlocksStreamPosition, std::ios_base::beg);
		ios.seekp(DataBlocksStreamPosition, std::ios_base::beg);
	}

	inline void SeekToEnd(std::iostream& ios) const
	{
		ios.seekg(GridEndPosition, std::ios_base::beg);
		ios.seekp(GridEndPosition, std::ios_base::beg);
	}
};
