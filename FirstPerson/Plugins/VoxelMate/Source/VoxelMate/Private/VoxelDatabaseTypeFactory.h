#pragma once
#include "VoxelDatabaseStatics.h"
#include <boost/shared_ptr.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/concepts.hpp>

class ArrayDevice;
class ArchiveDevice;
class FStreamWriter;
class FStreamReader;

template<typename VoxelDatabaseType>
struct FVoxelDatabaseTypeFactory
{
    typedef typename VoxelDatabaseType ValueType;
    typedef typename boost::shared_ptr<ValueType> ValueTypePtr;
    typedef typename boost::shared_ptr<const ValueType> ValueTypeConstPtr;
    ValueTypePtr ValuePtr;

    static const TArray<FString>& GetRegisteredTypeNames()
    {
        return FVoxelDatabaseTypeFactory<VoxelDatabaseType>::RegisteredTypeNames;
    }

    static void RegisterSupportedTypes();
    static void UnregisterSupportedTypes();

protected:
    static bool IsInitialized;
    static TArray<FString> RegisteredTypeNames;
    static bool IsRegisteredType(const FString& TypeName);
    static ValueTypePtr CreateType(const FString& TypeName);
};

class ArrayDevice : public boost::iostreams::device<boost::iostreams::seekable, ANSICHAR>
{
public:
	ArrayDevice(TArray<char_type>& Arr)
		: Array(Arr), Pos(Array.Num() > 0 ? 0 : -1)
	{}

	SSIZE_T GetPos()
	{
		return Pos;
	}

	std::streamsize read(char_type* Dest, std::streamsize Count)
	{
		check(Count > 0);
		const int32& Num = Array.Num();
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>(Pos, Num, 0, MAX_int32);

		const SSIZE_T Amount = FMath::Min((SSIZE_T)Count, (SSIZE_T)Num - Pos);
		if (Amount > 0)
		{
			const char_type* Src = Array.GetData() + (int32)Pos;
			FMemory::Memcpy(Dest, Src, Amount);
			Pos += Amount;
			return (std::streamsize)Amount;
		}
		return -1; //EOF
	}

	std::streamsize write(const char_type* Src, std::streamsize Count)
	{
		check(Count > 0);
		const int32& Num = Array.Num();
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>((SSIZE_T)Num + (SSIZE_T)Count, 0, MAX_int32);

		int32 Start = -1;
		if (Pos == Num)
		{
			Start = Array.AddUninitialized((int32)Count);
		}
		else if (Pos < Num)
		{
			Start = (int32)Pos + 1;
			Array.InsertUninitialized(Start, (int32)Count);
		}

		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>(Pos + (SSIZE_T)Count, 0, MAX_int32);
		VoxelDatabaseStatics::CheckSizeTypeRangeU<int32>((SIZE_T)Count, 0, MAX_int32);
		Pos += (SSIZE_T)Count;

		char_type* Dest = Array.GetData() + Start;
		FMemory::Memcpy(Dest, Src, (SIZE_T)Count);
		return Count;
	}

	boost::iostreams::stream_offset seek(boost::iostreams::stream_offset Offset, std::ios_base::seekdir Way)
	{
		const int32& Num = Array.Num();

		boost::iostreams::stream_offset Next;
		if (Way == std::ios_base::beg)
		{
			Next = Offset;
		}
		else if (Way == std::ios_base::cur)
		{
			Next = (boost::iostreams::stream_offset)Pos + Offset;
		}
		else if (Way == std::ios_base::end)
		{
			check(Num > -1);
			Next = (boost::iostreams::stream_offset)Num + Offset - 1;
		}
		else
		{
			check(false);
		}
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>((SSIZE_T)Next, 0, Num);

		Pos = (SSIZE_T)Next;
		return Pos;
	}

protected:
	SSIZE_T Pos;
	TArray<char_type>& Array;
};

class ArchiveDevice : public boost::iostreams::device<boost::iostreams::seekable, ANSICHAR>
{
public:
	ArchiveDevice(FArchive& Archive)
		: Ar(Archive), Pos((SSIZE_T)Ar.Tell())
	{}

	SSIZE_T GetPos()
	{
		return Pos;
	}

	VOXELMATEINLINE static void SerializeCh(FArchive& Ar, ANSICHAR& ch)
	{
		Ar << reinterpret_cast<uint8&>(ch);
	}

	std::streamsize read(char_type* Dest, std::streamsize Count)
	{
		check(Count > 0);
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>((SSIZE_T)Count, 0, MAX_int32);
		check(Ar.IsLoading());

		if (Ar.IsLoading())
		{
			ANSICHAR NextByte;
			for (int32 i = 0; i < (int32)Count; ++i)
			{
				SerializeCh(Ar, NextByte);
				*(Dest + i) = NextByte;
			}
			Pos += (SSIZE_T)Count;
			check(Pos == (SSIZE_T)Ar.Tell());
			return Count;
		}
		return -1; //EOF
	}

	std::streamsize write(const char_type* Src, std::streamsize Count)
	{
		check(Count > 0);
		const int64& ArPos = Ar.Tell();
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int32>((SSIZE_T)ArPos + (SSIZE_T)Count, 0, MAX_int32);
		check(Ar.IsSaving());

		if (Ar.IsSaving())
		{
			for (int32 i = 0; i < (int32)Count; ++i)
			{
				SerializeCh(Ar, *const_cast<char_type*>(Src + i));
			}
			Pos += (SSIZE_T)Count;
			check(Pos == (SSIZE_T)Ar.Tell());
			return Count;
		}
		//TODO what to return at failure?
		//return Count;
		return 0;
	}

	boost::iostreams::stream_offset seek(boost::iostreams::stream_offset Offset, std::ios_base::seekdir Way)
	{
		const int64& Size = Ar.TotalSize();

		boost::iostreams::stream_offset Next;
		if (Way == std::ios_base::beg)
		{
			Next = Offset;
		}
		else if (Way == std::ios_base::cur)
		{
			Next = (boost::iostreams::stream_offset)Pos + Offset;
		}
		else if (Way == std::ios_base::end)
		{
			check(Size > -1);
			Next = (boost::iostreams::stream_offset)Size + Offset - 1;
		}
		else
		{
			check(false);
		}
		VoxelDatabaseStatics::CheckSizeTypeRangeS<int64>((SSIZE_T)Next, 0, Size);

		Pos = (SSIZE_T)Next;
		Ar.Seek(Pos);
		return Pos;
	}

protected:
	SSIZE_T Pos;
	FArchive& Ar;
};

class FStreamWriter : public FArchive
{
public:
	FStreamWriter(TArray<ANSICHAR>& DestBytes, bool bIsPersistent = false, const FName InArchiveName = NAME_None)
		: FArchive()
		, Bytes(DestBytes)
		, ArchiveName(InArchiveName)
		, Device(*this)
		, Stream(Device)
	{
		ArIsSaving = true;
		ArIsPersistent = bIsPersistent;
	}

	virtual FString GetArchiveName() const override { return *FString::Printf(TEXT("FStreamWriter %s"), *ArchiveName.ToString()); }

	using FArchive::operator<<; // For visibility of the overloads we don't override

	virtual FArchive& operator<<(class FName& N) override
	{
		FString StringName = N.ToString();
		*this << StringName;
		return *this;
	}

	virtual FArchive& operator<<(class UObject*& Res) override
	{
		// Not supported through this archive
		check(0);
		(void)Res;
		return *this;
	}

	std::ostream& GetStream()
	{
		return Stream;
	}

	virtual void Seek(int64 InPos) override
	{
		Stream.seekp((std::streampos)InPos);
	}

	virtual int64 Tell() override
	{
		return (int64)Stream.tellp();
	}

	virtual int64 TotalSize() override
	{
		return Bytes.Num();
	}

	virtual void Serialize(void* Data, int64 Count) override
	{
		const int64 Num = Bytes.Num();
		check(Count > 0 && Count <= (int64)MAX_int32 && Num + Count <= (int64)MAX_int32);
		if (Count > 0)
		{
			const int32 Index = Bytes.AddUninitialized((int32)Count);
			FMemory::Memcpy(&Bytes[Index], Data, Count);
		}
		//TODO set flags such as ArIsError when needed
	}

protected:
	TArray<ANSICHAR>& Bytes;
	const FName ArchiveName;
	ArchiveDevice Device;
	boost::iostreams::stream<ArchiveDevice> Stream;
};

class FStreamReader : public FArchive
{
public:
	FStreamReader(const TArray<ANSICHAR>& SrcBytes, bool bIsPersistent = false, const FName InArchiveName = NAME_None)
		: FArchive()
		, Bytes(SrcBytes)
		, ArchiveName(InArchiveName)
		, Device(*this)
		, Stream(Device)
	{
		ArIsLoading = true;
		ArIsPersistent = bIsPersistent;
	}

	virtual FString GetArchiveName() const override { return *FString::Printf(TEXT("FStreamReader %s"), *ArchiveName.ToString()); }

	using FArchive::operator<<; // For visibility of the overloads we don't override

	virtual FArchive& operator<<(class FName& N) override
	{
		FString StringName;
		*this << StringName;
		N = FName(*StringName);
		return *this;
	}

	virtual FArchive& operator<<(class UObject*& Res) override
	{
		// Not supported through this archive
		check(0);
		(void)Res;
		return *this;
	}

	std::istream& GetStream()
	{
		return Stream;
	}

	virtual void Seek(int64 InPos) override
	{
		Stream.seekg((std::streampos)InPos);
	}

	virtual int64 Tell() override
	{
		return (int64)Stream.tellg();
	}

	virtual int64 TotalSize() override
	{
		return Bytes.Num();
	}

	virtual void Serialize(void* Data, int64 Count) override
	{
		const SSIZE_T& CurrentIndex = Device.GetPos();
		check(Count > 0 && Count <= (int64)MAX_int32);
		if (Count > 0 && CurrentIndex < (int64)MAX_int32)
		{
			FMemory::Memcpy(Data, &Bytes[(int32)CurrentIndex], (SIZE_T)Count);
		}
		//TODO set flags such as ArIsError when needed
	}

protected:
	const TArray<ANSICHAR>& Bytes;
	const FName ArchiveName;
	ArchiveDevice Device;
	boost::iostreams::stream<ArchiveDevice> Stream;
};
