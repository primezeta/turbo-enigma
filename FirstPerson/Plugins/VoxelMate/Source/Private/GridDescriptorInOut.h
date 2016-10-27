#pragma once
#include "Platform.h"
#include "Engine.h" //TODO: Engine.h is included just for UENUM, UCLASS, etc. Find exact header for these defines

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
#include "VoxelDatabaseStatics.h"
#include "VoxelMateStreams.h"
#include "IoStreamArray.h"

//TODO Logging for FGridDescriptor
//DECLARE_LOG_CATEGORY_EXTERN(LogGridDescriptor, Log, All)

DECLARE_LOG_CATEGORY_EXTERN(LogVoxelDatabase, Log, All)

#undef verify
#undef check
#define verifyue4(expr)	{ if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } } 
#define checkue4(expr) { if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(expr); } }

struct FVoxelDatabaseConfiguration
{
    FVoxelDatabaseConfiguration(const FLibraryVersionId& libraryVersion, bool partialGridReading, EGridCompression fileCompression, const openvdb::Name& uuid)
        : LibraryVersion(libraryVersion), SupportsPartialGridReading(partialGridReading), FileCompression(fileCompression), UUID(uuid)
    {}

    //NOTE: Assuming the file version is >= 222. TODO Allow previous versions.
    FLibraryVersionId LibraryVersion;
    bool SupportsPartialGridReading;
    EGridCompression FileCompression;
    openvdb::Name UUID;
};

class FOnScopeExitLambda
{
    FOnScopeExitLambda(TFunctionRef<void(void)>&& lambda)
        : LambdaOnExit(lambda)
    {}

    ~FOnScopeExitLambda()
    {
        LambdaOnExit();
    }

private:
    TFunctionRef<void(void)> LambdaOnExit;
};

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

class FGridDescriptor
{
public:
    void ReadFrom(std::istream& InputStream)
    {
        //const uint32 FileVersion = InputStream.GetFileVersion(); TODO
        const uint32 FileVersion = 0;

        GridName = FGridDescriptor::StripSuffix(UniqueName);
        GridType = ReadString(InputStream);

        IsFloatSavedAsHalf = boost::ends_with(GridType, GridIOStatics::HalfFloatTypenameSuffix);
        if (IsFloatSavedAsHalf)
        {
            boost::erase_last(GridType, GridIOStatics::HalfFloatTypenameSuffix);
        }

        //Try to create the grid, adding to the grid descriptor map if it was able to be created
        ParentGridName = openvdb::Name("");
        if (FileVersion >= GridIOStatics::FileVersionGridInstancing)
        {
            ParentGridName = ReadString(InputStream);
        }

        IsGridTypeRegistered = GridBaseStatics::IsRegistered(GridType);
        if (IsGridTypeRegistered)
        {
            GridPtr = GridBaseStatics::CreateGrid(GridType);
            if (GridPtr != nullptr)
            {
                //Set the grid descriptor float-as-half flag as well as the metadata value for float-as-half
                GridPtr->setSaveFloatAsHalf(IsFloatSavedAsHalf);

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
                //FROM_GRID_DATABASE_STRING(GridType),
                //FROM_GRID_DATABASE_STRING(GridName));
            }
        }

        // Read in the offsets for grid start, data blocks, and grid end
        ReadBytes<int64>(InputStream, GridStartPosition);
        ReadBytes<int64>(InputStream, DataBlocksPosition);
        ReadBytes<int64>(InputStream, GridEndPosition);

        check(GridEndPosition > std::streamoff(-1));
    }

    void WriteTo(std::ostream& OutputStream, bool IsSeekable)
    {
        WriteGridDescriptorHeader(OutputStream);

        int64 CurrentOffset = IsSeekable ? int64(OutputStream.OutputTell()) : 0;

        WriteStreamPos(OutputStream);

        // Now we know the starting grid storage position
        if (IsSeekable)
        {
            GridStartPosition = OutputStream.OutputTell();
        }
    }

    uint64 BytesCount()
    {
        uint64 Count = UniqueName.size() + GridType.size() + ParentGridName.size();
        if (IsFloatSavedAsHalf)
        {
            Count += GridIOStatics::HalfFloatTypenameSuffix.size();
        }
        Count += sizeof(GridStartPosition) + sizeof(DataBlocksPosition) + sizeof(GridEndPosition);
    }

    //Serialized
	openvdb::Name UniqueName;
	openvdb::Name GridType;
	openvdb::Name ParentGridName;
	int64 GridStartPosition;
	int64 DataBlocksPosition;
	int64 GridEndPosition;

    //Not serialized
    openvdb::GridBase::Ptr GridPtr;
    openvdb::Name GridName; //Unique name with the suffix stripped
    bool IsFloatSavedAsHalf;
    bool IsGridTypeRegistered;

    FGridDescriptor()
        : IsFloatSavedAsHalf(false), IsGridTypeRegistered(false), GridStartPosition(std::streamoff(-1)), DataBlocksPosition(std::streamoff(-1)), GridEndPosition(std::streamoff(-1))
    {}

    /* Write out the metadata, transform, and topology of the grid associated to the grid descriptor */
    static inline int64 WriteGridMeta(FStream<std::ostream>& os, FGridDescriptor& GridDescriptor, bool areDatabaseStreamsSeekable)
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
            GridDescriptor.DataBlocksPosition = os.tellp();
        }

        return offsetPos;
    }

    /* Write out the grid data blocks to the output stream */
    static inline void WriteGridDataBlocks(FStream<std::ostream>& os, FGridDescriptor& GridDescriptor, bool areDatabaseStreamsSeekable, int64 offsetPos)
    {
        openvdb::GridBase& grid = *GridDescriptor.GridPtr;

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

	void WriteGridDescriptorHeader(std::ostream& os) const
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
        WriteString(os, ParentGridName);
	}

	void WriteStreamPos(std::ostream& os) const
	{
		os.write(reinterpret_cast<const char*>(&GridStartPosition), sizeof(int64));
		os.write(reinterpret_cast<const char*>(&DataBlocksPosition), sizeof(int64));
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
		is.seekg(DataBlocksPosition, std::ios_base::beg);
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
		os.seekp(DataBlocksPosition, std::ios_base::beg);
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
		ios.seekg(DataBlocksPosition, std::ios_base::beg);
		ios.seekp(DataBlocksPosition, std::ios_base::beg);
	}

	inline void SeekToEnd(std::iostream& ios) const
	{
		ios.seekg(GridEndPosition, std::ios_base::beg);
		ios.seekp(GridEndPosition, std::ios_base::beg);
	}
};
