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

class FVoxelDatabaseArchive : public FMemoryArchive, public FGridArchive
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
		: GridArchiveName(DatabaseName), IsDatabaseOpen(false)
	{
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

    const TArray<FString>& GetRegisteredGridTypeDisplayNames() const
    {
        return RegisteredGridTypeDisplayNames;
    }

    const TArray<FString>& GetRegisteredMetadataTypeDisplayNames() const
    {
        return RegisteredMetadataTypeDisplayNames;
    }

    const TArray<FString>& GetRegisteredTransformMapTypeDisplayNames() const
    {
        return RegisteredTransformMapTypeDisplayNames;
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
		bool containsGrid = FindGridDescriptor(gridName) != GridDescriptors.end();
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
			isMetadataValueFound = true;
		}
		UE_LOG(LogVoxelDatabase, Verbose, TEXT("%s"), MetaAttributes.ToString());
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
        const bool isOpen = IsByteStreamOpen(1);
		if (isOpen)
		{
			OutVoxelDatabaseMetadata.LibraryVersion = FGridArchive::libraryVersion();
            OutVoxelDatabaseMetadata.SupportsPartialGridReading = FGridArchive::isDelayedLoadingEnabled();
            OutVoxelDatabaseMetadata.FileCompression = (EGridCompression)FGridArchive::compression();
            OutVoxelDatabaseMetadata.UUID = FGridArchive::getUniqueTag();
		}
		return isOpen;
	}

	bool OpenDatabase()
	{
        //Add byte arrays for file header, file metadata, and grid descriptors
        //Open the first two byte streams which are used for the file header and file metadata

        bool isDatabaseOpen = IsDatabaseOpen;        
        if (!isDatabaseOpen)
        {
            const bool isDatabaseHeaderOpen = ReadDatabaseHeader();
            if (isDatabaseHeaderOpen)
            {
                const bool isDatabaseMetadataOpen = ReadDatabaseMetadata();
                //TODO log open status
                if (isDatabaseMetadataOpen)
                {
                    bool isDatabaseGridDescriptorsOpen = ReadDatabaseGridDescriptors();
                    //TODO log open status
                }
            }
            IsDatabaseOpen = isDatabaseHeaderOpen;
        }
		return IsDatabaseOpen;
	}

    void CloseDatabase()
    {
        CloseAllByteStreams();
        //TODO write changes
    }

protected:

	inline bool IsByteStreamInitialized(int32 StreamIndex) const
	{
        checkue4(StreamIndex > -1 && StreamIndex < DatabaseByteAccessors.Num());
        return DatabaseByteAccessors[StreamIndex] != nullptr;
	}

	inline bool IsByteStreamOpen(int32 StreamIndex) const
	{
        checkue4(StreamIndex > -1 && StreamIndex < DatabaseByteStreams.Num());
		IsByteStreamInitialized(StreamIndex) && DatabaseByteStreams[StreamIndex].is_open();
	}

    inline void EmptyByteStreams()
    {
        //TODO logging
        DatabaseBytes.Empty();
        DatabaseByteAccessors.Empty();
        DatabaseByteStreams.Empty();
    }

    inline int32 AddByteStream(uint64 BytesCount)
    {
        //if (BytesCount <= UINT32_MAX) {} TODO error handling
        int32 NextStreamIndex = DatabaseBytes.Num();
        if (!IsByteStreamInitialized(NextStreamIndex))
        {
            DatabaseBytes.AddDefaulted(1);

            DatabaseBytes[NextStreamIndex].AddDefaulted(BytesCount);
            DatabaseByteAccessors.AddDefaulted(1);
            DatabaseByteStreams.AddDefaulted(1);

            auto* DataStart = DatabaseBytes[NextStreamIndex].GetData();
            auto* DataEnd = DataStart + DatabaseBytes[NextStreamIndex].Num();
            checkue4(DataStart > DataEnd); //TODO error handling
            DatabaseByteAccessors[NextStreamIndex].reset(new boost::iostreams::array(DataStart, DataEnd));
            DatabaseByteStreams[NextStreamIndex].open(*DatabaseByteAccessors[NextStreamIndex]);
        }
        return IsByteStreamOpen(NextStreamIndex);
    }

    inline void CloseByteStream(int32 StreamIndex)
    {
        checkue4(StreamIndex > 0 && StreamIndex < DatabaseByteStreams.Num());
        DatabaseByteStreams[StreamIndex].close();
    }

    inline void CloseAllByteStreams()
    {
        for (auto i = DatabaseByteStreams.CreateIterator(); i; ++i)
        {
            i->close();
        }
        EmptyByteStreams();
    }

	/* If the database is not already open, reset the stream buffer to the beginning of the file and read the header and grid descriptors */
	bool ReadDatabaseHeader()
	{
		checkue4(!IsByteStreamOpen(0));

		bool isGridArchiveOpen = false;
        if (!IsByteStreamOpen(0) && AddByteStream(DatabaseHeaderBytesCount))
        {
            //Read the file header as the first item in the buffer
            FGridArchive::readHeader(DatabaseByteStreams[0]);

            //openvdb::io::StreamMetadata
            //Initialize I/O stream metadata
            //StreamPtr.reset(new FStreamMetadata<std::iostream>(this)); TODO Add mechanism to reset the stream metadata
            FGridArchive::setFormatVersion(DatabaseByteStreams[0]);
            FGridArchive::setLibraryVersion(DatabaseByteStreams[0]);
            FGridArchive::setDataCompression(DatabaseByteStreams[0]);

            //DO NOT set a mapped file.
            //If a mapped file is set, leaf nodes would attempt to read in their buffers via the boost mapped file contained in the unchangeable Impl.
            //In addition to not setting the mapped file, OPENVDB_2_ABI_COMPATIBLE must be defined to avoid other mapped file operations under the covers of openvdb::io.
            //The mapped file pointer is null by default as long as openvdb::io::setMappedFilePtr isn't called.
            //For reference, if a mapped file was desired and #ifndef OPENVDB_2_ABI_COMPATIBLE, this would be the next call:
            //openvdb::io::setMappedFilePtr(StreamBuffer, mFileMapping);
         
            isGridArchiveOpen = true;
        }
		return isGridArchiveOpen;
	}

    bool ReadDatabaseMetadata()
    {
        checkue4(!IsByteStreamOpen(1));

        bool isGridArchiveOpen = false;
        if (!IsByteStreamOpen(1) && AddByteStream(DatabaseMetadataBytesCount))
        {
            //Read the file-level metadata
            DatabaseFileMetadata.clearMetadata();
            DatabaseFileMetadata.readMeta(DatabaseByteStreams[1]);
        }
    }

    bool ReadDatabaseGridDescriptors()
    {
        checkue4(!IsByteStreamOpen(2));

        bool isGridArchiveOpen = false;
        if (!IsByteStreamOpen(2) && AddByteStream(DatabaseGridDescriptorsBytesCount))
        {
            //Read the number of grids then for each grid read just the grid descriptor (not grid data)
            NumGrids = FGridArchive::readGridCount(DatabaseByteStreams[2]);
            if (NumGrids > 0)
            {
                GridDescriptors.clear();
                
                for (int32 i = 0; i < NumGrids; ++i)
                {
                    bool isGridStreamOpen = false;

                    FGridDescriptorNameMapCIter GridDescriptorIter = FGridDescriptor::ReadAndAddNextGridDescriptor<boost::iostreams::stream<boost::iostreams::array>>(DatabaseByteStreams[2], GridDescriptors);
                    if (GridDescriptorIter != GridDescriptors.end())
                    {
                        FGridDescriptor& gridDescriptor = *(GridDescriptorIter->second);
                        if (gridDescriptor.GridEndPosition > gridDescriptor.GridStartPosition)
                        {
                            isGridStreamOpen = AddByteStream(gridDescriptor.GridEndPosition - gridDescriptor.GridStartPosition);
                        }
                    }

                    if (isGridStreamOpen)
                    {
                        //TODO log success
                    }
                    else
                    {
                        //TODO log failure
                    }
                }
            }
            //TODO log number of grids
        }
    }

    void ReadAllGridsPartial()
    {
        for (FGridDescriptorNameMapCIter i = GridDescriptors.begin(); i != GridDescriptors.end(); ++i)
        {
            const FGridDescriptor& gridDescriptor = *i->second;
            const int32 byteStreamIndex = gridDescriptor.GridDescriptorIndex + 3;
            FStream<boost::iostreams::stream<boost::iostreams::array>>& inputStream = DatabaseByteStreams[byteStreamIndex];
            GridBaseStatics::GridBase& grid = *gridDescriptor.GridPtr;

            FGridArchive::readGridCompression(inputStream);
            grid.readMeta(inputStream);

            const int32 FormatVersion = GridIOStatics::GetFormatVersion(DatabaseByteStreams[byteStreamIndex]);
            const bool isFileVersionForGridInstancing = FormatVersion > GridIOStatics::FileVersionGridInstancing;
            if (isFileVersionForGridInstancing)
            {
                grid.readTransform(inputStream);
                const bool isGridInstance = gridDescriptor.IsGridInstance();
                if (isGridInstance)
                {
                    grid.readTopology(inputStream);
                }
            }
            else
            {
                grid.readTopology(inputStream);
                grid.readTransform(inputStream);
            }
        }
    }

    template<typename TreeType>
    void ReadGridBuffers(const FString& GridName)
    {
        FGridDescriptorNameMapCIter gridIter = FindGridDescriptor(GridName);
        if (gridIter != GridDescriptors.end())
        {
            const FGridDescriptor& gridDescriptor = *gridIter->second;
            const int32 byteStreamIndex = gridDescriptor.GridDescriptorIndex + 3;
            FStream<boost::iostreams::stream<boost::iostreams::array>>& inputStream = DatabaseByteStreams[byteStreamIndex];
            const FGrid<TreeType>::FPtr gridPtr = boost::static_pointer_cast<FGrid<TreeType>>(gridDescriptor.GridPtr);
            checkue4(gridPtr != nullptr);
            const FGrid<TreeType>& grid = *gridPtr;
            grid.readBuffers(inputStream, grid.saveFloatAsHalf());
        }
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
					const FGridName candidateName = i->second->UniqueName;
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
    bool IsDatabaseOpen;
    uint64 DatabaseHeaderBytesCount;
    uint64 DatabaseMetadataBytesCount;
    uint64 DatabaseGridDescriptorsBytesCount;
    uint64 DatabaseGridBytesCount;

	TArray<TArray<char>> DatabaseBytes;
    TArray<boost::shared_ptr<boost::iostreams::array>> DatabaseByteAccessors;
    TArray<FStream<boost::iostreams::stream<boost::iostreams::array>>> DatabaseByteStreams;

	TMap<FGridName, uint64> MaxGridSizeByType;

	FString GridArchiveName;
	int32 NumGrids;

	FGridDescriptorNameMap GridDescriptors;
	FMetaMap DatabaseFileMetadata;

    TArray<FString> RegisteredGridTypeDisplayNames;
    TArray<FString> RegisteredMetadataTypeDisplayNames;
    TArray<FString> RegisteredTransformMapTypeDisplayNames;
};
