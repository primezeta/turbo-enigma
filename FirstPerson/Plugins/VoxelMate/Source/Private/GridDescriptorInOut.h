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

    FLibraryVersionId(const FLibraryVersionId& Version)
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
    
    inline void SetFileVersion(uint32 version)
    {
        openvdb::io::StreamMetadata::setFileVersion(version);
    }

    inline void SetLibraryVersion(const FLibraryVersionId& libraryVersion)
    {
        openvdb::io::StreamMetadata::setLibraryVersion(libraryVersion);
    }

    inline void SetGridCompression(EGridCompression gridCompression)
    {
        openvdb::io::StreamMetadata::setCompression((uint32)gridCompression);
    }

    inline void SetGridClass(EGridClass gridClass)
    {
        openvdb::io::StreamMetadata::setGridClass((uint32)gridClass);
    }

    inline void SetBackgroundValuePtr(const void* backgroundPtr)
    {
        openvdb::io::StreamMetadata::setBackgroundPtr(backgroundPtr);
    }

    inline void SetIsHalfFloat(bool isHalfFloat)
    {
        openvdb::io::StreamMetadata::setHalfFloat(isHalfFloat);
    }

    inline void SetWriteGridStats(bool writeGridStats)
    {
        openvdb::io::StreamMetadata::setWriteGridStats(writeGridStats);
    }

	inline FGridDatabaseString ToStr() const
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
	typedef boost::shared_ptr<FStream<StreamType>> FPtr;

	FStream()
		: FStreamMetadata(*this)
	{}

    operator FStream<std::istream>&() const //TODO: Should be explicit?
    {
        return static_cast<FStream<std::istream>&>(*this);
    }

    operator FStream<std::ostream>&() const //TODO: Should be explicit?
    {
        return static_cast<FStream<std::ostream>&>(*this);
    }
};

//extern const char* HALF_FLOAT_TYPENAME_SUFFIX; FFFFUUUUU can't get this to link from the anonymous namespace
namespace GridIOStatics
{
    enum
    {
        FileVersionRootNodeMap             = openvdb::OPENVDB_FILE_VERSION_ROOTNODE_MAP,
        FileVersionInternalNodeCompression = openvdb::OPENVDB_FILE_VERSION_INTERNALNODE_COMPRESSION,
        FileVersionSimplifiedGridTypename  = openvdb::OPENVDB_FILE_VERSION_SIMPLIFIED_GRID_TYPENAME,
        FileVersionGridInstancing          = openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING,
        FileVersionBoolLeafOptimization    = openvdb::OPENVDB_FILE_VERSION_BOOL_LEAF_OPTIMIZATION,
        FileVersionBoostUuid               = openvdb::OPENVDB_FILE_VERSION_BOOST_UUID,
        FileVersionNoGridMap               = openvdb::OPENVDB_FILE_VERSION_NO_GRIDMAP,
        FileVersionNewTransform            = openvdb::OPENVDB_FILE_VERSION_NEW_TRANSFORM,
        FileVersionSelectiveCompression    = openvdb::OPENVDB_FILE_VERSION_SELECTIVE_COMPRESSION,
        FileVersionFloatFrustumBbox        = openvdb::OPENVDB_FILE_VERSION_FLOAT_FRUSTUM_BBOX,
        FileVersionNodeMaskCompression     = openvdb::OPENVDB_FILE_VERSION_NODE_MASK_COMPRESSION,
        FileVersionBloscCompression        = openvdb::OPENVDB_FILE_VERSION_BLOSC_COMPRESSION,
        FileVersionPointIndexGrid          = openvdb::OPENVDB_FILE_VERSION_POINT_INDEX_GRID,
    };

    const static FGridDatabaseString HalfFloatTypenameSuffix = "_HalfFloat";
    //const static FGridDatabaseString HalfFloatTypenameSuffix = HALF_FLOAT_TYPENAME_SUFFIX;;

    uint32 GetFormatVersion(std::ios_base& ios)
    {
        return openvdb::io::getFormatVersion(ios);
    }

    void SetDataCompression(std::ios_base& ios, uint32 compressionFlags)
    {
        openvdb::io::setDataCompression(ios, compressionFlags);
    }

    void SetWriteGridStatsMetadata(std::ios_base& ios, bool areGridStatsWritten)
    {
        openvdb::io::setWriteGridStatsMetadata(ios, areGridStatsWritten);
    }

    uint32 GetDataCompression(std::ios_base& ios)
    {
        return openvdb::io::getDataCompression(ios);
    }

    FGridDatabaseString CompressionToString(uint32 compressionFlags)
    {
        return openvdb::io::compressionToString(compressionFlags);
    }
}

namespace MetadataStatics
{
	static inline void ClearRegistry()
	{
		openvdb::Metadata::clearRegistry();
	}
}

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
};

class TreeBaseStatics : public openvdb::TreeBase
{
public:
    typedef openvdb::TreeBase::Ptr FPtr;
};

class GridBaseStatics : public openvdb::GridBase
{
public:
    typedef openvdb::GridBase::Ptr FPtr;

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
        openvdb::GridBase::clearRegistry();
	}

	static inline FPtr CreateGrid(const FGridName& type)
	{
		return openvdb::GridBase::createGrid(type);
	}

	static inline bool IsRegistered(const FGridName& type)
	{
		return openvdb::GridBase::isRegistered(type);
	}

	static inline void RegisterGrid(const FGridName& type, openvdb::GridBase::GridFactory factory)
	{
		openvdb::GridBase::registerGrid(type, factory);
	}

	static inline void UnregisterGrid(const FGridName& type)
	{
		openvdb::GridBase::unregisterGrid(type);
	}

    static FGridDatabaseString GridClassToString(EGridClass GridClass)
    {
        return openvdb::GridBase::gridClassToString((openvdb::GridClass)GridClass);
    }

    static FGridDatabaseString GridClassToMenuName(EGridClass GridClass)
    {
        return openvdb::GridBase::gridClassToMenuName((openvdb::GridClass)GridClass);
    }

    static EGridClass StringToGridClass(const FGridDatabaseString& Str)
    {
        return (EGridClass)openvdb::GridBase::stringToGridClass(Str);
    }

    static FGridDatabaseString VecTypeToString(EVectorTypeClass VecType)
    {
        return openvdb::GridBase::vecTypeToString((openvdb::VecType)VecType);
    }

    static FGridDatabaseString VecTypeExamples(EVectorTypeClass VecType)
    {
        return openvdb::GridBase::vecTypeExamples((openvdb::VecType)VecType);
    }

    static FGridDatabaseString VecTypeDescription(EVectorTypeClass VecType)
    {
        return openvdb::GridBase::vecTypeDescription((openvdb::VecType)VecType);
    }

    static EVectorTypeClass StringToVecType(const FGridDatabaseString& Str)
    {
        return (EVectorTypeClass)openvdb::GridBase::stringToVecType(Str);
    }
protected:
    //Protected constructor to prevent instantiation because this class is not meant to be used.
    //It exists solely to rename openvdb::GridBase and to grant access to protected openvdb::GridBase static functions without breaking the openvdb::Grid hierarchy.
    GridBaseStatics() {}
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
    typedef boost::shared_ptr<FMetadataUnknown> FPtr;
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
	int64 GridStartPosition;
	int64 DataBlocksStreamPosition;
	int64 GridEndPosition;

    static int32 NextGridDescriptorIndex;
    int32 GridDescriptorIndex;

    /* Write out the grid associated to the grid descriptor */
    static inline void WriteGrid(FStream<std::ostream>& os, FGridDescriptor& GridDescriptor, bool areDatabaseStreamsSeekable)
    {
        // Write out the descriptor's header information (grid name and type)
        GridDescriptor.WriteGridHeader(os);

        // Save the curent stream position as postion to where the offsets for
        // this GridDescriptor will be written to.
        int64 offsetPos = 0;
        if (areDatabaseStreamsSeekable)
        {
            offsetPos = (int64)os.tellp();
        }

        // Write out the offset information. At this point it will be incorrect.
        // But we need to write it out to move the stream head forward.
        GridDescriptor.WriteStreamPos(os);

        // Now we know the starting grid storage position.
        if (areDatabaseStreamsSeekable)
        {
            GridDescriptor.GridStartPosition = os.tellp();
        }

        // Save the compression settings for this grid.
        //setGridCompression(os, *grid); TODO

        // Save the grid's metadata and transform.
        openvdb::GridBase& grid = *GridDescriptor.GridPtr;
        const bool isGridStatsMetadataWritten = os.GetShouldWriteGridStats();
        if (!isGridStatsMetadataWritten)
        {
            grid.writeMeta(os);
        }
        else
        {
            // Compute and add grid statistics metadata.
            const uint32 compressionFlags = GridIOStatics::GetDataCompression(os);
            openvdb::GridBase::Ptr copyOfGridPtr = grid.copyGrid(); // shallow copy
            openvdb::GridBase& copyOfGrid = *copyOfGridPtr;
            copyOfGrid.addStatsMetadata();
            copyOfGrid.insertMeta(GridBaseStatics::MetaNameFileCompression, openvdb::StringMetadata(GridIOStatics::CompressionToString(compressionFlags)));
            copyOfGrid.writeMeta(os);
        }
        grid.writeTransform(os);

        // Save the grid's structure.
        grid.writeTopology(os);

        // Now we know the grid block storage position.
        if (areDatabaseStreamsSeekable)
        {
            GridDescriptor.DataBlocksStreamPosition = os.tellp();
        }

        // Save out the data blocks of the grid.
        grid.writeBuffers(os);

        // Now we know the end position of this grid.
        if (areDatabaseStreamsSeekable)
        {
            GridDescriptor.GridEndPosition = os.tellp();

            // Now, go back to where the Descriptor's offset information is written
            // and write the offsets again.
            os.seekp(offsetPos, std::ios_base::beg);
            GridDescriptor.WriteStreamPos(os);

            // Now seek back to the end.
            GridDescriptor.SeekToEnd(os);
        }
    }

    /* Write out the grid descriptor as a grid instance to the output stream */
    static inline void WriteGridInstance(FStream<std::ostream>& os, FGridDescriptor& GridDescriptor, bool areDatabaseStreamsSeekable)
    {
        // Write out the descriptor's header information (grid name and type)
        GridDescriptor.WriteGridHeader(os);

        // Save the curent stream position as postion to where the offsets for
        // this GridDescriptor will be written to.
        int64 offsetPos = 0;
        if (areDatabaseStreamsSeekable)
        {
            offsetPos = (int64)os.tellp();
        }

        // Write out the offset information. At this point it will be incorrect.
        // But we need to write it out to move the stream head forward.
        GridDescriptor.WriteStreamPos(os);

        // Now we know the starting grid storage position.
        if (areDatabaseStreamsSeekable)
        {
            GridDescriptor.GridStartPosition = os.tellp();
        }

        // Save the compression settings for this grid.
        //setGridCompression(os, *grid); TODO

        // Save the grid's metadata and transform.
        openvdb::GridBase& grid = *GridDescriptor.GridPtr;
        grid.writeMeta(os);
        grid.writeTransform(os);

        // Now we know the end position of this grid.
        if (areDatabaseStreamsSeekable)
        {
            GridDescriptor.GridEndPosition = os.tellp();

            // Now, go back to where the Descriptor's offset information is written
            // and write the offsets again.
            os.seekp(offsetPos, std::ios_base::beg);
            GridDescriptor.WriteStreamPos(os);

            // Now seek back to the end.
            GridDescriptor.SeekToEnd(os);
        }
    }

	/* Read the next grid descriptor into the input stream and return the stream position of the next grid descriptor */
	static FGridDescriptorNameMapCIter ReadAndAddNextGridDescriptor(FStream<std::istream>& is, FGridDescriptorNameMap &OutGridDescriptors)
	{
		auto& InputStream = static_cast<std::istream&>(is);
		FGridDescriptorNameMapIter GridDescriptorIter = OutGridDescriptors.end();
        const uint32 FileVersion = is.GetFileVersion();

		//Unique name is read alone previously to ReadGridDescriptor
		const FGridName uniqueName = FGridDatabaseString(ReadString(InputStream));
		FGridDatabaseString gridType;
		bool isFloatSavedAsHalf = false;
		ReadGridTypeAndStripHalfSavedAsFloatSuffix(is, gridType, isFloatSavedAsHalf);
        
        //Try to create the grid, adding to the grid descriptor map if it was able to be created
        FGridDatabaseString parentName = FGridDatabaseString("");
        if (FileVersion >= GridIOStatics::FileVersionGridInstancing)
        {
            parentName = ReadString(InputStream);
        }

        const int64 nextGridPos = TryCreateGridAndAddGridDescriptor(is, uniqueName, gridType, parentName, isFloatSavedAsHalf, OutGridDescriptors, GridDescriptorIter);
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
	static inline void ReadGridTypeAndStripHalfSavedAsFloatSuffix(FStream<std::istream>& is, FGridDatabaseString& OutGridType, bool& OutIsFloatSavedAsHalf)
	{
		auto& InputStream = static_cast<std::istream&>(is);
		OutGridType = ReadString(InputStream);
		OutIsFloatSavedAsHalf = boost::ends_with(OutGridType, GridIOStatics::HalfFloatTypenameSuffix);
		if (OutIsFloatSavedAsHalf)
		{
			boost::erase_last(OutGridType, GridIOStatics::HalfFloatTypenameSuffix);
		}
	}

	/*Attempt to create the grid via the base class, from which a call to the registered factory is made by the grid registry.
	* If successful create a grid descriptor and add it to the grid descriptor map and return an iter to that grid descriptor.
	*/
	static inline int64 TryCreateGridAndAddGridDescriptor(FStream<std::istream>& is, const FGridDatabaseString& GridUniqueName, const FGridDatabaseString& GridTypeWithoutFloatAsHalfSuffix, const FGridDatabaseString& GridParentName, bool IsFloatHalf, FGridDescriptorNameMap &OutGridDescriptors, FGridDescriptorNameMapIter& OutGridDescriptorIter)
	{
		auto& InputStream = static_cast<std::istream&>(is);
        int64 nextGridStreamPos = std::streamoff(-1);

        const bool IsGridTypeRegistered = GridBaseStatics::IsRegistered(GridTypeWithoutFloatAsHalfSuffix);
        if (IsGridTypeRegistered)
        {
            GridBaseStatics::FPtr gridPtr = GridBaseStatics::CreateGrid(GridTypeWithoutFloatAsHalfSuffix);
            if (gridPtr != nullptr)
            {
                OutGridDescriptorIter = OutGridDescriptors.emplace(GridUniqueName, boost::shared_ptr<FGridDescriptor>(new FGridDescriptor()));
                check(OutGridDescriptorIter != OutGridDescriptors.end()); //TODO error handling
                FGridDescriptor& GridDescriptor = *OutGridDescriptorIter->second;

                GridDescriptor.GridPtr = gridPtr;
                GridDescriptor.UniqueName = GridUniqueName;
                GridDescriptor.GridName = FGridDescriptor::StripSuffix(GridUniqueName);
                GridDescriptor.GridType = GridTypeWithoutFloatAsHalfSuffix;
                GridDescriptor.ParentGridName = GridParentName;

                //Set the grid descriptor float-as-half flag as well as the metadata value for float-as-half
                GridDescriptor.IsFloatSavedAsHalf = IsFloatHalf;
                GridDescriptor.GridPtr->setSaveFloatAsHalf(IsFloatHalf);

                // Read in the offsets for grid start, data blocks, and grid end
                ReadGridStreamPositions<std::istream>(is, GridDescriptor.GridStartPosition, GridDescriptor.DataBlocksStreamPosition, GridDescriptor.GridEndPosition);
                nextGridStreamPos = GridDescriptor.GridEndPosition;

                //TODO Not sure yet if the following will be set here
                //GridPtr->setCreator()
                //GridPtr->setGridClass()
                //GridPtr->setIsInWorldSpace()
                //GridPtr->setVectorType()

                GridDescriptor.GridDescriptorIndex = FGridDescriptor::NextGridDescriptorIndex++;

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
            ReadNextGridStreamPosition(is, nextGridStreamPos);
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

    static inline FGridName AddSuffix(const FGridName& name, int32 suffix)
    {
        return openvdb::io::GridDescriptor::addSuffix(name, suffix);
    }

	void WriteGridHeader(std::ostream& os) const
	{
		WriteString(os, UniqueName);
		if (IsFloatSavedAsHalf)
		{
			WriteString(os, GridType + GridIOStatics::HalfFloatTypenameSuffix);
		}
		else
		{
			WriteString(os, GridType);
		}
	}

	void WriteStreamPos(std::ostream& os) const
	{
		os.write(reinterpret_cast<const char*>(&GridStartPosition), sizeof(int64));
		os.write(reinterpret_cast<const char*>(&DataBlocksStreamPosition), sizeof(int64));
		os.write(reinterpret_cast<const char*>(&GridEndPosition), sizeof(int64));
	}

	bool IsGridInstance() const
	{
		return !ParentGridName.empty();
	}

	inline void SeekToGrid(std::istream& is) const
	{
		is.seekg(GridStartPosition, std::ios_base::beg);
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
		os.seekp(GridStartPosition, std::ios_base::beg);
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
		ios.seekg(GridStartPosition, std::ios_base::beg);
		ios.seekp(GridStartPosition, std::ios_base::beg);
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
