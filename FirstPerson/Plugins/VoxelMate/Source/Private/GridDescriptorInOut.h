#pragma once
#include "Platform.h"
#include <openvdb/openvdb.h>
#include <openvdb/io/io.h>
#include <openvdb/util/Name.h>
#include <boost/algorithm/string/predicate.hpp> // for boost::ends_with()
#include <boost/algorithm/string/erase.hpp> // for boost::erase_last()

//TODO Logging for FGridDescriptor
//DECLARE_LOG_CATEGORY_EXTERN(LogGridDescriptor, Log, All)

//The half-float typename suffix is declared in GridDescriptor.cc
extern const char* HALF_FLOAT_TYPENAME_SUFFIX; //TODO Move this to it's own .h file

struct FGridDescriptor
{
public:
	typedef boost::shared_ptr<FGridDescriptor> Ptr;
	
	openvdb::Name UniqueName;
	openvdb::Name GridName;
	openvdb::Name GridType;
	openvdb::Name ParentGridName;
	openvdb::Name AllocationName;
	bool IsFloatSavedAsHalf;
	boost::int64_t GridStreamPosition;
	boost::int64_t DataBlocksStreamPosition;
	boost::int64_t GridEndPosition;

	static void WriteString(std::ostream& os, const openvdb::Name &str)
	{
		uint32 size = uint32(str.size());
		os.write(reinterpret_cast<char*>(&size), sizeof(uint32));
		os.write(&str[0], size);
	}

	static void ReadString(std::istream& is, openvdb::Name &OutStr)
	{
		uint32 size = 0;
		is.read(reinterpret_cast<char*>(&size), sizeof(uint32));
		if (size > 0)
		{
			is.read(&OutStr[0], size);
		}
		else
		{
			OutStr = std::string(size, ' ');
		}
	}

	static bool ReadGridDescriptor(std::istream& is, const std::string &uniqueName, FGridDescriptor &OutGridDescriptor)
	{
		//Unique name is read alone previously to ReadGridDescriptor
		OutGridDescriptor.UniqueName = uniqueName;
		OutGridDescriptor.GridName = openvdb::io::GridDescriptor::stripSuffix(OutGridDescriptor.UniqueName);
		
		ReadString(is, OutGridDescriptor.GridType);
		OutGridDescriptor.IsFloatSavedAsHalf = boost::ends_with(OutGridDescriptor.GridType, HALF_FLOAT_TYPENAME_SUFFIX);
		if (OutGridDescriptor.IsFloatSavedAsHalf)
		{
			boost::erase_last(OutGridDescriptor.GridType, HALF_FLOAT_TYPENAME_SUFFIX);
		}

		const uint32 formatVersion = openvdb::io::getFormatVersion(is);
		if (formatVersion >= openvdb::OPENVDB_FILE_VERSION_GRID_INSTANCING)
		{
			ReadString(is, OutGridDescriptor.ParentGridName);
		}

		// Read in the offsets.
		is.read(reinterpret_cast<char*>(&OutGridDescriptor.GridStreamPosition), sizeof(boost::int64_t));
		is.read(reinterpret_cast<char*>(&OutGridDescriptor.DataBlocksStreamPosition), sizeof(boost::int64_t));
		is.read(reinterpret_cast<char*>(&OutGridDescriptor.GridEndPosition), sizeof(boost::int64_t));

		return openvdb::GridBase::isRegistered(OutGridDescriptor.GridType);
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
		os.write(reinterpret_cast<const char*>(&GridStreamPosition), sizeof(boost::int64_t));
		os.write(reinterpret_cast<const char*>(&DataBlocksStreamPosition), sizeof(boost::int64_t));
		os.write(reinterpret_cast<const char*>(&GridEndPosition), sizeof(boost::int64_t));
	}

	bool IsGridInstance() const
	{
		return !ParentGridName.empty();
	}

	void SeekToGrid(std::iostream& ios) const
	{
		ios.seekg(GridStreamPosition, std::ios_base::beg);
		ios.seekp(GridStreamPosition, std::ios_base::beg);
	}

	void SeekToBlocks(std::iostream& ios) const
	{
		ios.seekg(DataBlocksStreamPosition, std::ios_base::beg);
		ios.seekp(DataBlocksStreamPosition, std::ios_base::beg);
	}

	void SeekToEnd(std::iostream& ios) const
	{
		ios.seekg(GridEndPosition, std::ios_base::beg);
		ios.seekp(GridEndPosition, std::ios_base::beg);
	}
};

namespace openvdb {
	OPENVDB_USE_VERSION_NAMESPACE
	namespace OPENVDB_VERSION_NAME {
		namespace io {
			typedef std::multimap<openvdb::Name, FGridDescriptor::Ptr> GridDescriptorNameMap;
			typedef GridDescriptorNameMap::const_iterator GridDescriptorNameMapCIter;
		}
	}
}
