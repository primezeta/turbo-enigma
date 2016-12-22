#pragma once
//#include "VoxelDatabaseBulkData.generated.h"

//UCLASS()
//class UVoxelDatabaseBulkData : public UObject
//{
//	GENERATED_BODY()
//
//public:
//	UVoxelDatabaseBulkData()
//	{
//		BulkData.SetBulkDataFlags(BULKDATA_SerializeCompressed | BULKDATA_SerializeCompressedBitWindow); //TODO bulk data flags
//	}
//
//	void InitializeBulkFile(const FString& DataFilename, const void* const Data, int64 DataSizeBytes)
//	{
//		//TODO bulk data flags
//		//// The bulk data cannot be removed if we are loading from source file
//		//BulkData.ClearBulkDataFlags(BULKDATA_SingleUse);
//
//		BulkData.Lock(LOCK_READ_WRITE);
//		void* const LockedData = BulkData.Realloc(DataSizeBytes);
//		FMemory::Memcpy(LockedData, Data, DataSizeBytes);
//		TUniquePtr<FArchive> Writer = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*DataFilename, 0));
//		Writer->Serialize(LockedData, DataSizeBytes);
//		BulkData.Unlock();
//	}
//
//	void InitializeReader(const FString& DataFilename)
//	{
//		//TODO bulk data flags
//		//// The bulk data cannot be removed if we are loading from source file
//		//BulkData.ClearBulkDataFlags(BULKDATA_SingleUse);
//
//		if (!DataFileReader.IsValid())
//		{
//			DataFileReader = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*DataFilename, 0));
//		}
//
//		const int64 DataSizeBytes = DataFileReader->TotalSize();
//		BulkData.Lock(LOCK_READ_WRITE);
//		void* const Data = BulkData.Realloc(DataSizeBytes);
//		DataFileReader->Serialize(Data, DataSizeBytes);
//		BulkData.Unlock();
//	}
//
//	void InitializeWriter(const FString& DataFilename, int64 DataSizeBytes)
//	{
//		//TODO bulk data flags
//		//// The bulk data cannot be removed if we are loading from source file
//		//BulkData.ClearBulkDataFlags(BULKDATA_SingleUse);
//
//		if (!DataFileWriter.IsValid())
//		{
//			DataFileWriter = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*DataFilename, 0));
//		}
//
//		BulkData.Lock(LOCK_READ_WRITE);
//		void* const LockedData = BulkData.Lock(0); //TODO bulk data lock flags
//		DataFileWriter->Serialize(LockedData, DataSizeBytes);
//		BulkData.Unlock();
//	}
//
//	const void* LockRead(int32& DataSizeBytes) const
//	{
//		CriticalSection.Lock();
//		DataSizeBytes = BulkData.GetBulkDataSize();
//		const void* Data = BulkData.LockReadOnly();
//		return Data;
//	}
//
//	void* LockWrite(int32& DataSizeBytes) const
//	{
//		CriticalSection.Lock();
//		DataSizeBytes = BulkData.GetBulkDataSize();
//		void* Data = BulkData.Lock(0); //TODO bulk data lock flags
//		return Data;
//	}
//
//	void Unlock() const
//	{
//		bool bWasLoaded = BulkData.IsBulkDataLoaded();
//		int32 BulkDataSize = BulkData.GetBulkDataSize();
//		BulkData.Unlock();
//		CriticalSection.Unlock();
//	}
//
//	int32 GetBulkDataSize() const
//	{
//		return BulkData.GetBulkDataSize();
//	}
//
//	int32 GetBulkDataSizeOnDisk() const
//	{
//		return BulkData.GetBulkDataSizeOnDisk();
//	}
//
//	virtual void Serialize(FArchive& Ar) override
//	{
//		Super::Serialize(Ar);
//		BulkData.Serialize(Ar, this);
//
//		if (!GIsEditor && Ar.IsLoading())
//		{
//			//TODO bulk data flags
//			//BulkData.SetBulkDataFlags(BULKDATA_SingleUse);
//		}
//	}
//
//private:
//	TUniquePtr<FArchive> DataFileReader;
//	TUniquePtr<FArchive> DataFileWriter;
//	FByteBulkData BulkData;
//	mutable FCriticalSection CriticalSection;
//};
