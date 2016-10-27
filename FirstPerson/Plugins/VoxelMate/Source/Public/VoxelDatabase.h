#pragma once
#include "EngineMinimal.h"
#include "VoxelData.h"
#include "EngineGridTypes.h"
#include "VoxelDatabaseStatics.h"
#include "ArchiveGrid.h"
#include "ArchiveMetaMap.h"
#include "VoxelDatabase.generated.h"

UCLASS()
class UVoxelDatabase : public UObject
{
    GENERATED_BODY()

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
    UVoxelDatabase(const FObjectInitializer& ObjectInitializer)
		: AreDatabaseStreamsSeekable(true), IsGridInstancingEnabled(true)
	{
	}

    FORCEINLINE friend FArchive& operator<<(FArchive& Ar, UVoxelDatabase& VoxelDatabase)
    {
        Ar << VoxelDatabase.DatabaseHeader;
        Ar << VoxelDatabase.DatabaseMetadata;
        Ar << VoxelDatabase.Grids;
        return Ar;
    }

    virtual void Serialize(FArchive& Ar) override
    {
        Ar << *this;
        Super::Serialize(Ar);
    }

    void GetGridNames(TArray<FString> &OutGridNames) const
    {
        //for (FGridDescriptorNameMapCIter i = GridDescriptors.begin(); i != GridDescriptors.end(); ++i)
        //{
        //    if (i->second != nullptr)
        //    {
        //        OutGridNames.Add(FROM_GRID_DATABASE_STRING(i->first));
        //    }
        //}
    }

    const TArray<FString>& GetRegisteredGridTypeDisplayNames() const
    {
        return FGridFactory::RegisteredTypeDisplayNames;
    }

    const TArray<FString>& GetRegisteredMetadataTypeDisplayNames() const
    {
        return FMetaValueFactory::RegisteredTypeDisplayNames;
    }

    const TArray<FString>& GetRegisteredTransformMapTypeDisplayNames() const
    {
        return FTransformMapFactory::RegisteredTypeDisplayNames;
    }

    void InitializeStandardTypes();

    template<typename ValueType>
    bool AddGrid(const FString& GridName, bool IsFloatHalf)
    {
        bool isGridAdded = false;
        //if (IsDatabaseOpen)
        //{
        //    int32 gridByteCount = 0;
        //    FGridName uniqueGridName = GridName;
        //    MakeAndInsertUniqueName(uniqueGridName);
        //    const openvdb::Name typeName = openvdb::typeNameAsString<ValueType>();

        //    gridByteCount += uniqueGridName.size() + typeName.size();
        //    if (IsFloatHalf)
        //    {
        //        //Reserve space for the half-as-float suffix without actually appending it to the type name
        //        gridByteCount += GridIOStatics::HalfFloatTypenameSuffix.size();
        //    }

        //    const openvdb::Name parentName = ""; //No parent name. To add an instanced grid, use AddGridInstance
        //    const int32 StreamIndex = AddByteStream(gridByteCount);
        //    //TODO!!!!**** Need to have a version of TryCreateGridAndAddGridDescriptor that operates on an output stream
        //    //const int64 nextGridPos = FGridDescriptor::TryCreateGridAndAddGridDescriptor(DatabaseByteStreams[StreamIndex], uniqueGridName, typeName, parentName, IsFloatHalf, GridDescriptors)
        //    
        //}
        //else
        //{
        //    //TODO logging and error handling
        //}
        return isGridAdded;
    }

    template<typename ValueType>
    bool AddGridInstance(const FString& GridName, const FString& ParentGridName, bool IsFloatHalf)
    {
        bool isGridInstanceAdded = false;
        //if (IsDatabaseOpen)
        //{
        //    if (FileVersion >= GridIOStatics::FileVersionGridInstancing)
        //    {
        //    }
        //}
        //else
        //{
        //    //TODO logging and error handling
        //}
        return isGridInstanceAdded;
    }

	bool ContainsGrid(const FString& gridName) const
	{
		//bool containsGrid = FindGridDescriptor(gridName) != GridDescriptors.end();
		//return containsGrid;
        return false; //TODO
	}

	/* Return the database metadata */
	//bool GetDatabaseConfiguration(FVoxelDatabaseConfiguration& OutVoxelDatabaseConfiguration) const
	//{
  //      const bool IsOpen = DatabaseHeader.IsOpen();
		//if (IsOpen)
		//{
  //          OutVoxelDatabaseConfiguration.LibraryVersion = DatabaseHeader.libraryVersion();
  //          OutVoxelDatabaseConfiguration.SupportsPartialGridReading = DatabaseHeader.isDelayedLoadingEnabled();
  //          OutVoxelDatabaseConfiguration.FileCompression = (EGridCompression)DatabaseHeader.compression();
  //          OutVoxelDatabaseConfiguration.UUID = DatabaseHeader.getUniqueTag();
		//}
	//	return IsOpen;
	//}

	bool OpenDatabase()
	{
        //Add byte arrays for file header, file metadata, and grid descriptors
        //Open the first two byte streams which are used for the file header and file metadata

        bool isDatabaseOpen = false;
        //bool isDatabaseOpen = IsDatabaseOpen;
        //if (!isDatabaseOpen)
        //{
        //    const bool isDatabaseHeaderOpen = ReadDatabaseHeader();
        //    if (isDatabaseHeaderOpen)
        //    {
        //        const bool isDatabaseMetadataOpen = ReadDatabaseMetadata();
        //        IsDatabaseOpen = isDatabaseMetadataOpen;
        //        //TODO log open status
        //    }
        //}
		return isDatabaseOpen;
	}

    void CloseDatabase()
    {
        //TODO
    }
        
protected:

	/* If the database is not already open, reset the stream buffer to the beginning of the file and read the header and grid descriptors */
    inline bool ReadDatabaseHeader()
	{
		const bool IsOpen = false;
        return IsOpen;
	}

    inline void WriteDatabaseHeader()
    {
    }

    inline bool ReadDatabaseMetadata()
    {
        const bool IsOpen = false;
        return IsOpen;
    }

    inline bool WriteDatabaseMetadata()
    {
        const bool IsOpen = false;
        return IsOpen;
    }

    void WriteAllGrids()
    {
        //TODO
        //Write out the non-null grids with unique names constructed with a suffix corresponding to the count of that name if there are more than one
        //for (FGridDescriptorNameMapCIter i = GridDescriptors.begin(); i != GridDescriptors.end(); ++i)
        //{
        //    FGridDescriptor& gridDescriptor = *(i->second);
        //    if (gridDescriptor.GridPtr != nullptr && gridDescriptor.IsChanged)
        //    {
        //        const GridStatics::GridBase& grid = *gridDescriptor.GridPtr;

        //        // Ensure that the grid's descriptor has a unique grid name, by appending
        //        // a number to it if a grid with the same name was already written.
        //        // Always add a number if the grid name is empty, so that the grid can be
        //        // properly identified as an instance parent, if necessary.
        //        FGridName uniqueGridName = grid.getName();
        //        MakeAndInsertUniqueName(uniqueGridName);

        //        //Check if this grid's tree is shared with a grid that has already been written.
        //        const openvdb::TreeBase* treePtr = &(grid.baseTree());
        //        FTreeMapIter treeIter = TreeMap.find(treePtr);

        //        const bool isInstance = ((treeIter != TreeMap.end()) && (treeIter->second.IsFloatSavedAsHalf == gridDescriptor.IsFloatSavedAsHalf));
        //        if (IsGridInstancingEnabled && isInstance)
        //        {
        //            // This grid's tree is shared with another grid that has already been written.
        //            // Get the name of the other grid.
        //            gridDescriptor.ParentGridName = treeIter->second.UniqueName;
        //            // Write out this grid's descriptor and metadata, but not its tree.
        //            FGridDescriptor::WriteGridInstance(DatabaseByteStreams[2], gridDescriptor, AreDatabaseStreamsSeekable);
        //        }
        //        else
        //        {
        //            // Write out the grid descriptor and its associated grid header, stream positions, metadata, transform, and topology
        //            const int64 offsetPos = FGridDescriptor::WriteGridMeta(DatabaseByteStreams[2], gridDescriptor, AreDatabaseStreamsSeekable);
        //            FGridDescriptor::WriteGridDataBlocks(DatabaseByteStreams[2], gridDescriptor, AreDatabaseStreamsSeekable, offsetPos);
        //            // Record the grid's tree pointer so that the tree doesn't get written
        //            // more than once.
        //            TreeMap[treePtr] = gridDescriptor;
        //        }
        //    }
        //}
    }

    void MakeAndInsertUniqueName(FGridName& InOutGridName)
    {
        const FGridName originalName = InOutGridName;

        //if (InOutGridName.empty() || GridNamesHistogram[InOutGridName] > 1)
        //{
        //    InOutGridName = FGridDescriptor::AddSuffix(InOutGridName, 0);
        //}

        //for (int32 n = 1; GridNamesHistogram.find(InOutGridName) != GridNamesHistogram.end(); ++n)
        //{
        //    InOutGridName = FGridDescriptor::AddSuffix(originalName, n);
        //}
        //UniqueNames.insert(InOutGridName);
    }

    void ReadAllGridsMeta()
    {
        //for (FGridDescriptorNameMapCIter i = GridDescriptors.begin(); i != GridDescriptors.end(); ++i)
        //{
        //    const FGridDescriptor& gridDescriptor = *i->second;
        //    const int32& byteStreamIndex = gridDescriptor.GridDescriptorStreamIndex;
        //    FStream<boost::iostreams::stream<boost::iostreams::array>>& inputStream = DatabaseByteStreams[byteStreamIndex];
        //    GridStatics::GridBase& grid = *gridDescriptor.GridPtr;

        //    FGridArchive::readGridCompression(inputStream);
        //    grid.readMeta(inputStream);

        //    const int32 FormatVersion = GridIOStatics::GetFormatVersion(DatabaseByteStreams[byteStreamIndex]);
        //    const bool isFileVersionForGridInstancing = FormatVersion > GridIOStatics::FileVersionGridInstancing;
        //    const bool isFileVersionForNoGridMap = FormatVersion < GridIOStatics::FileVersionNoGridMap;

        //    if (isFileVersionForGridInstancing)
        //    {
        //        //Read transform then topology
        //        //If the file version supports grid instancing then only read the topology for child grids
        //        grid.readTransform(inputStream);
        //        const bool isGridInstance = gridDescriptor.IsGridInstance();
        //        if (isGridInstance)
        //        {
        //            grid.readTopology(inputStream);
        //        }
        //    }
        //    else
        //    {
        //        //Read topology then transform
        //        grid.readTopology(inputStream);
        //        grid.readTransform(inputStream);
        //    }

        //    if (isFileVersionForNoGridMap)
        //    {
        //        //This file version does not store the grid name as grid metadata so set the name here
        //        if (grid.getName().empty())
        //        {
        //            grid.setName(gridDescriptor.GridName);
        //        }
        //    }
        //}
    }

    template<typename TreeType>
    void ReadGridBuffers(const FString& GridName)
    {
        //FGridDescriptorNameMapCIter gridIter = FindGridDescriptor(GridName);
        //if (gridIter != GridDescriptors.end())
        //{
        //    const FGridDescriptor& gridDescriptor = *gridIter->second;
        //    const int32& byteStreamIndex = gridDescriptor.GridDescriptorStreamIndex;
        //    FStream<boost::iostreams::stream<boost::iostreams::array>>& inputStream = DatabaseByteStreams[byteStreamIndex];
        //    const FGrid<TreeType>::FPtr gridPtr = boost::static_pointer_cast<FGrid<TreeType>>(gridDescriptor.GridPtr);
        //    checkue4(gridPtr != nullptr);
        //    const FGrid<TreeType>& grid = *gridPtr;
        //    grid.readBuffers(inputStream, grid.saveFloatAsHalf());
        //}
    }

	FGridDescriptorNameMapCIter FindGridDescriptor(const FString &gridName) const
	{
		//const FGridName name = TO_GRID_DATABASE_STRING(gridName);
		//const FGridName uniqueName = FGridDescriptor::StringAsUniqueName(name);

		//// Find all descriptors with the given grid name.
  //      
		//std::pair<FGridDescriptorNameMapCIter, FGridDescriptorNameMapCIter> range = GridDescriptors.equal_range(name);

		//if (range.first == range.second)
		//{
		//	// If no descriptors were found with the given grid name, the name might have
		//	// a suffix ("name[N]").  In that case, remove the "[N]" suffix and search again.
		//	range = GridDescriptors.equal_range(FGridDescriptor::StripSuffix(uniqueName).c_str());
		//}

		//const size_t count = size_t(std::distance(range.first, range.second));
		//if (count > 1 && name == uniqueName)
		//{
		//	UE_LOG(LogVoxelDatabase, Error, TEXT("%s has more than one grid named %s"), *GetGridDatabaseName(), *gridName);
		//}

		//FGridDescriptorNameMapCIter foundGridDescriptor = GridDescriptors.end();
		//if (count > 0)
		//{
		//	if (name == uniqueName)
		//	{
		//		// If the given grid name is unique or if no "[N]" index was given,
		//		// use the first matching descriptor.
		//		foundGridDescriptor = range.first;
		//	}
		//	else
		//	{
		//		// If the given grid name has a "[N]" index, find the descriptor
		//		// with a matching unique name.
		//		for (FGridDescriptorNameMapCIter i = range.first; i != range.second; ++i)
		//		{
		//			const FGridName candidateName = i->second->UniqueName;
		//			if (candidateName == uniqueName || candidateName == name)
		//			{
		//				foundGridDescriptor = i;
		//				break;
		//			}
		//		}
		//	}
		//}
		//return foundGridDescriptor;
	}

    
    UPROPERTY()
        FString GridDatabaseName;
    UPROPERTY()
        bool AreDatabaseStreamsSeekable;
    UPROPERTY()
        bool IsGridInstancingEnabled;

protected:
    FVoxelDatabaseHeader DatabaseHeader;
    TMap<FString, FMetaValueFactory> DatabaseMetadata;
    TMap<FString, FGridFactory> Grids;
};
