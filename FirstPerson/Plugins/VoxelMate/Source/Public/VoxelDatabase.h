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
#include <boost/iostreams/stream.hpp>

//5,4,3 is the standard openvdb tree configuration
typedef openvdb::tree::Tree4<FVoxelData, 5, 4, 3>::Type TreeType;
//class FVoxelGrid;
//class FVoxelMetaData;
//class FVoxelTreeData;
//class FVoxelTransform;

template<typename VoxelDataTreeType>
class FVoxelDatabaseBase : public FArchive
{
public:
	typedef typename VoxelDataTreeType TreeType;
	typedef openvdb::Grid<VoxelDataTreeType> GridType;

	FVoxelDatabaseBase()
	{
		openvdb::initialize();
		if (!GridType::isRegistered())
		{
			GridType::registerGrid();
		}
		//if (!GridType::isRegistered())
		//{
		//	GridType::registerGrid();
		//}
	}
};

class FVoxelDatabase : public FVoxelDatabaseBase<TreeType>,
	//public boost::iostreams::stream<boost::iostreams::stream_buffer<boost::iostreams::array>>,
	public boost::iostreams::stream<boost::iostreams::array>,
	public openvdb::io::Archive
{
public:

	FVoxelDatabase(const FString &name, bool delayLoad)
		: SharedMemory(boost::interprocess::open_or_create, TCHAR_TO_UTF8(*name), sizeof(GridType)),
		MemoryBuffer(static_cast<char*>(SharedMemory.get_address()), SharedMemory.get_size()),
		//StreamBuffer(MemoryBuffer),
		boost::iostreams::stream<boost::iostreams::array>(MemoryBuffer)
	{
		auto p = SharedMemory.construct<GridType>(TCHAR_TO_UTF8(*FString(name+TEXT("_grid"))));
		GridPtr = static_cast<GridType*>(p());
		checkue4(GridPtr != nullptr);
	}

private:
	GridType * GridPtr;
	//boost::iostreams::stream_buffer<boost::iostreams::array> StreamBuffer;
	boost::iostreams::array MemoryBuffer;
	boost::interprocess::managed_shared_memory SharedMemory;
};