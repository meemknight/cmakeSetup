///////////////////////////////////////////
//do not remove this notice
//(c) Luta Vlad
// 
// safeSave 1.0.0
// 
///////////////////////////////////////////

#pragma once
#include <fstream>
#include <vector>

#ifdef _MSC_VER
#pragma warning( disable : 26812 )
#endif

namespace sfs
{
	enum Errors : int
	{
		noError,
		couldNotOpenFinle,
		fileSizeDitNotMatch,
		checkSumFailed,
		couldNotMakeBackup,
		readBackup,
	};
	
	const char* getErrorString(Errors e);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors readEntireFile(void* data, size_t size, const char* name, bool shouldMatchSize, int *bytesRead = nullptr);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors readEntireFileWithCheckSum(void* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFileWithCheckSum(const void* data, size_t size, const char* name);

	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const std::vector<char>& data, const char* name);
	
	//can return error: couldNotOpenFinle
	Errors writeEntireFile(const void*data, size_t size, const char* name);

	//can return error: couldNotOpenFinle, 
	//	couldNotMakeBackup (if reportnotMakingBackupAsAnError is true, but will still save the first file)
	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError);

	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed, 
	//	readBackup (if reportLoadingBackupAsAnError but data will still be loaded with the backup)
	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError);

	//same as safeLoad but only loads the backup file.
	//can return error: couldNotOpenFinle, fileSizeDitNotMatch, checkSumFailed
	Errors safeLoadBackup(void* data, size_t size, const char* nameWithoutExtension);

#if defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__

	struct FileMapping
	{
		void* pointer = {};
		size_t size = 0;
		struct
		{
			void* fileHandle = 0;
			void* fileMapping = 0;
		}internal = {};
	};

#elif defined __linux__

	struct FileMapping
	{
		void* pointer = {};
		size_t size = 0;
		struct
		{
			int fd = 0;
		}internal = {};
	};

#endif

	//can return error: couldNotOpenFinle
	Errors openFileMapping(FileMapping& fileMapping, const char* name, size_t size, bool createIfNotExisting);

	void closeFileMapping(FileMapping& fileMapping);

};