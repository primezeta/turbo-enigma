#pragma once
#include "Platform.h"
#include <iostream>
#include <openvdb/openvdb.h>
#include <openvdb/util/Name.h>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string/predicate.hpp> // for boost::ends_with()
#include <boost/algorithm/string/erase.hpp> // for boost::erase_last()

//TODO make namespace 

template<typename Type>
static inline void ReadBytes(std::istream& is, Type &OutValue)
{
	is.read(reinterpret_cast<char*>(&OutValue), sizeof(Type));
}

template<typename Type>
static inline void WriteBytes(std::ostream& os, const Type &InValue)
{
    os.write(reinterpret_cast<char*>(&InValue), sizeof(Type));
}

static inline void WriteString(std::ostream& os, const std::string &str)
{
	uint32 size = uint32(str.size());
	os.write(reinterpret_cast<char*>(&size), sizeof(uint32));
	os.write(&str[0], size);
}

static inline const char* ReadString(std::istream& is)
{
	uint32 size = 0;
	is.read(reinterpret_cast<char*>(&size), sizeof(uint32));
	char* str = nullptr;
	if (size > 0)
	{
		is.read(str, size);
	}
	return str;
}
