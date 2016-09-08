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
#include "EngineGridTypes.h"
#include "VoxelMateStreams.h"

//TODO Logging for FGridDescriptor
//DECLARE_LOG_CATEGORY_EXTERN(LogGridDescriptor, Log, All)

//The half-float typename suffix is declared in GridDescriptor.cc
extern const char* HALF_FLOAT_TYPENAME_SUFFIX; //TODO Move this to it's own .h file

struct FLibraryVersionId : public openvdb::VersionId
{
	FLibraryVersionId()
		: openvdb::VersionId()
	{}

	FLibraryVersionId(uint32 Major, uint32 Minor)
		: openvdb::VersionId(Major, Minor)
	{}

	FLibraryVersionId(const openvdb::VersionId& Version)
		: openvdb::VersionId(Version.first, Version.second)
	{}
};

class FStreamMetadata : public openvdb::io::StreamMetadata
{
public:
	typedef boost::shared_ptr<FStreamMetadata> FPtr;

	const int64 MagicNumber; //TODO make static

	FStreamMetadata(std::ios_base& FileStream)
		: openvdb::io::StreamMetadata(FileStream),
		MagicNumber(int64((uint64(openvdb::OPENVDB_MAGIC) << 32) | (uint64(openvdb::OPENVDB_MAGIC))))
	{}

	inline void TransferTo(std::ios_base& FileStream) const
	{
		openvdb::io::StreamMetadata::transferTo(FileStream);
	}

	inline uint32 GetFileVersion() const
	{
		return openvdb::io::StreamMetadata::fileVersion();
	}

	inline FLibraryVersionId GetLibraryVersion() const
	{
		return FLibraryVersionId(openvdb::io::StreamMetadata::libraryVersion());
	}

	inline uint32 GetCompression() const
	{
		return openvdb::io::StreamMetadata::compression();
	}

	inline uint32 GetGridClass() const
	{
		return openvdb::io::StreamMetadata::gridClass();
	}

	inline const void* GetBackgroundValuePtr() const
	{
		return openvdb::io::StreamMetadata::backgroundPtr();
	}

	inline bool IsHalfFloat() const
	{
		return openvdb::io::StreamMetadata::halfFloat();
	}

	inline bool GetShouldWriteGridStats() const
	{
		return openvdb::io::StreamMetadata::writeGridStats();
	}

	inline const FMetaMap& GetGridMetadata() const
	{
		return openvdb::io::StreamMetadata::gridMetadata();
	}

	inline FMetaMap& GetGridMetadata()
	{
		return openvdb::io::StreamMetadata::gridMetadata();
	}

	inline const FAuxDataMap& GetAuxData() const
	{
		return openvdb::io::StreamMetadata::auxData();
	}

	inline FAuxDataMap& GetAuxData()
	{
		return openvdb::io::StreamMetadata::auxData();
	}

	inline FGridDatabaseString Str() const
	{
		return openvdb::io::StreamMetadata::str();
	}

	inline FString ToString() const
	{
		return FString(FROM_GRID_DATABASE_STRING(openvdb::io::StreamMetadata::str()));
	}
};

template<typename StreamType>
class FStream : public StreamType, public FStreamMetadata
{
public:
	typedef boost::shared_ptr<FStream> FPtr;

	FStream()
		: FStreamMetadata(*this)
	{}
};

namespace GridIOStatics
{
	const static uint32 FileVersionGridInstancing = openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING;
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
};

class FGridArchive : public openvdb::io::Archive
{
public:
	typedef boost::shared_ptr<FGridArchive> FPtr;
	typedef const FPtr FConstPtr;
};

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
	static inline bool IsRegistered()
	{
		return FGrid<TreeType>::isRegistered();
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

	GridBaseStatics::FPtr GridPtr; //Important! When serializing a grid descriptor, do not serialize the grid
	FGridDatabaseString UniqueName;
	FGridDatabaseString GridName; //Grid name is not archived, rather it is the unique name with the suffix stripped
	FGridDatabaseString GridType;
	FGridDatabaseString ParentGridName;
	bool IsFloatSavedAsHalf;
	int64 GridStreamPosition;
	int64 DataBlocksStreamPosition;
	int64 GridEndPosition;

	/* Read the next grid descriptor into the input stream and return the stream position of the next grid descriptor */
	template<typename StreamType>
	static FGridDescriptorNameMapCIter ReadAndAddNextGridDescriptor(FStream<StreamType>& is, FGridDescriptorNameMap &OutGridDescriptors)
	{
		auto& InputStream = static_cast<StreamType&>(is);
		FGridDescriptorNameMapIter GridDescriptorIter = OutGridDescriptors.end();
        const uint32 FileVersion = is.GetFileVersion();

		//Unique name is read alone previously to ReadGridDescriptor
		const FGridName uniqueName = FGridDatabaseString(ReadString(InputStream));
		FGridDatabaseString gridType;
		bool isFloatSavedAsHalf = false;
		ReadGridTypeAndStripHalfSavedAsFloatSuffix<StreamType>(is, gridType, isFloatSavedAsHalf);
        
        //Try to create the grid, adding to the grid descriptor map if it was able to be created
        FGridDatabaseString parentName = FGridDatabaseString("");
        if (FileVersion >= GridIOStatics::FileVersionGridInstancing)
        {
            parentName = ReadString(InputStream);
        }

        const int64 nextGridPos = TryCreateGridAndAddGridDescriptor<StreamType>(is, uniqueName, gridType, parentName, isFloatSavedAsHalf, OutGridDescriptors, GridDescriptorIter);
        if (nextGridPos > std::streamoff(-1))
        {
            //Seek to the next grid
            InputStream.seekg(nextGridPos, std::ios_base::beg);
        }
        else
        {
            //TODO error handling
        }
		return GridDescriptorIter;
	}

	/* Read the next string in as the grid type and return the grid type with the half-as-float suffix removed (if it exists).
	* Also return true if there was a suffix that got removed.
	*/
	template<typename StreamType>
	static inline void ReadGridTypeAndStripHalfSavedAsFloatSuffix(FStream<StreamType>& is, FGridDatabaseString& OutGridType, bool& OutIsFloatSavedAsHalf)
	{
		auto& InputStream = static_cast<StreamType&>(is);		
		OutGridType = ReadString(InputStream);
		OutIsFloatSavedAsHalf = boost::ends_with(OutGridType, HALF_FLOAT_TYPENAME_SUFFIX);
		if (OutIsFloatSavedAsHalf)
		{
			boost::erase_last(OutGridType, HALF_FLOAT_TYPENAME_SUFFIX);
		}
	}

	/*Attempt to create the grid via the base class, from which a call to the registered factory is made by the grid registry.
	* If successful create a grid descriptor and add it to the grid descriptor map and return an iter to that grid descriptor.
	*/
	template<typename StreamType>
	static inline int64 TryCreateGridAndAddGridDescriptor(FStream<StreamType>& is, const FGridDatabaseString& GridUniqueName, const FGridDatabaseString& GridTypeWithoutFloatAsHalfSuffix, const FGridDatabaseString& GridParentName, bool IsFloatHalf, FGridDescriptorNameMap &OutGridDescriptors, FGridDescriptorNameMapIter& OutGridDescriptorIter)
	{
		auto& InputStream = static_cast<StreamType&>(is);
        int64 nextGridStreamPos = std::streamoff(-1);

        const bool IsGridTypeRegistered = FGridBase::isRegistered(GridTypeWithoutFloatAsHalfSuffix);
        if (IsGridTypeRegistered)
        {
            GridBaseStatics::FPtr gridPtr = FGridBase::createGrid(GridTypeWithoutFloatAsHalfSuffix);
            if (gridPtr != nullptr)
            {
                OutGridDescriptorIter = OutGridDescriptors.emplace(GridUniqueName, FGridDescriptor());
                check(OutGridDescriptorIter != OutGridDescriptors.end()); //TODO error handling
                FGridDescriptor& GridDescriptor = OutGridDescriptorIter->second;

                GridDescriptor.GridPtr = gridPtr;
                GridDescriptor.UniqueName = GridUniqueName;
                GridDescriptor.GridName = FGridDescriptor::StripSuffix(GridUniqueName);
                GridDescriptor.GridType = GridTypeWithoutFloatAsHalfSuffix;
                GridDescriptor.ParentGridName = GridParentName;

                //Set the grid descriptor float-as-half flag as well as the metadata value for float-as-half
                GridDescriptor.IsFloatSavedAsHalf = IsFloatHalf;
                GridDescriptor.GridPtr->setSaveFloatAsHalf(IsFloatHalf);

                // Read in the offsets for grid start, data blocks, and grid end
                ReadGridStreamPositions<StreamType>(is, GridDescriptor.GridStreamPosition, GridDescriptor.DataBlocksStreamPosition, GridDescriptor.GridEndPosition);
                nextGridStreamPos = GridDescriptor.GridEndPosition;

                //TODO Not sure yet if the following will be set here
                //GridPtr->setCreator()
                //GridPtr->setGridClass()
                //GridPtr->setIsInWorldSpace()
                //GridPtr->setVectorType()
                //TODO Logging for FGridDescriptor
                //UE_LOG(LogGridDescriptor, Display, TEXT("Created %s grid %s"),
                //FROM_GRID_DATABASE_STRING(GridDescriptor.GridType),
                //FROM_GRID_DATABASE_STRING(GridDescriptor.GridName));
            }
            else
            {
                //TODO Logging for FGridDescriptor
                //UE_LOG(LogGridDescriptor, Error, TEXT("Failed to create %s grid %s"),
                //FROM_GRID_DATABASE_STRING(GridDescriptor.GridType),
                //FROM_GRID_DATABASE_STRING(GridDescriptor.GridName));
            }
        }
        else
        {
            OutGridDescriptorIter = OutGridDescriptors.end();
            ReadNextGridStreamPosition<StreamType>(is, nextGridStreamPos);
            //The grid could not be read because it wasn't registered.
            //Log the unreadable grid and continue as normal.
            //TODO Logging for FGridDescriptor
            //UE_LOG(LogGridDescriptor, Error, TEXT("Could not create grid %s because grid type %s is not registered"),
            //FROM_GRID_DATABASE_STRING(GridDescriptor.GridName),
            //FROM_GRID_DATABASE_STRING(GridDescriptor.GridType));
        }
        return nextGridStreamPos;
	}
    
    template<typename StreamType>
    static inline void ReadGridStreamPositions(FStream<StreamType>& is, int64& OutGridStartPos, int64& OutDataBlocksPos, int64& OutGridEndPos)
    {
        auto& InputStream = static_cast<StreamType&>(is);
        ReadBytes<int64>(InputStream, OutGridStartPos);
        ReadBytes<int64>(InputStream, OutDataBlocksPos);
        ReadBytes<int64>(InputStream, OutGridEndPos);
    }

    template<typename StreamType>
    static inline void ReadNextGridStreamPosition(FStream<StreamType>& is, int64& OutNextGridStartPos)
    {
        auto& InputStream = static_cast<StreamType&>(is);
        ReadBytes<int64>(InputStream, OutNextGridStartPos); //read and discard...
        ReadBytes<int64>(InputStream, OutNextGridStartPos); //read and discard...
        ReadBytes<int64>(InputStream, OutNextGridStartPos); //use the third value
    }

	static inline FGridName StringAsUniqueName(const FGridName& name)
	{
		return openvdb::io::GridDescriptor::stringAsUniqueName(name);
	}

	static inline FGridName StripSuffix(const FGridName& name)
	{
		return openvdb::io::GridDescriptor::stripSuffix(name);
	}

	void WriteGridHeader(std::ostream& os) const
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

	void WriteStreamPos(std::ostream& os) const
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
