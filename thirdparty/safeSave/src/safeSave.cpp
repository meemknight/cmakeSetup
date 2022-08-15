///////////////////////////////////////////
//do not remove this notice
//(c) Luta Vlad
// 
// safeSave 1.0.0
// 
///////////////////////////////////////////

#include <safeSave.h>

#if defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__
#define NOMINMAX 
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#elif defined __linux__
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#endif


namespace sfs
{

	constexpr const char* errorsString[] =
	{
		"noError",
		"couldNotOpenFinle",
		"fileSizeDitNotMatch",
		"checkSumFailed",
		"couldNotMakeBackup",
		"readBackup",
	};

	const char* getErrorString(Errors e)
	{
		if (e >= 0 && e < sizeof(errorsString) / sizeof(errorsString[0]))
		{
			return errorsString[e];
		}
		else
		{
			return "unknown error";
		}
	}

	Errors readEntireFile(std::vector<char>& data, const char* name)
	{
		data.clear();
		std::ifstream f(name, std::ios::binary);

		if(f.is_open())
		{
			f.seekg(0, std::ios_base::end);
			size_t size = f.tellg();
			f.seekg(0, std::ios_base::beg);

			data.resize(size);

			f.read(&data[0], size);

			return noError;
		}else
		{
			return couldNotOpenFinle;
		}
	}

	Errors readEntireFile(void* data, size_t size, const char* name, bool shouldMatchSize, int* bytesRead)
	{
		if (bytesRead)
		{
			*bytesRead = 0;
		}

		std::ifstream f(name, std::ios::binary);
		if (f.is_open())
		{
			f.seekg(0, std::ios_base::end);
			size_t readSize = f.tellg();
			f.seekg(0, std::ios_base::beg);

			if (shouldMatchSize)
			{
				if (readSize != size)
				{
					return fileSizeDitNotMatch;
				}
				else
				{
					f.read((char*)data, readSize);
					if (bytesRead)
					{
						*bytesRead = readSize;
					}

					return noError;
				}
			}
			else
			{
				f.read((char*)data, std::min(size, readSize));

				if (bytesRead)
				{
					*bytesRead = std::min(size, readSize);
				}

				return noError;
			}
		}
		else
		{
			return couldNotOpenFinle;
		}
	}
	
	using HashType = unsigned long long;
	
	//https://stackoverflow.com/questions/34595/what-is-a-good-hash-function
	unsigned long long fnv_hash_1a_64(const void* key, int len)
	{
		const unsigned char* p = (const unsigned char*)key;
		unsigned long long h = 0xcbf29ce484222325ULL;
		for (int i = 0; i < len; i+=4)
		{
			h = (h ^ p[i + 0]) * 0x100000001b3ULL;
			h = (h ^ p[i + 1]) * 0x100000001b3ULL;
			h = (h ^ p[i + 2]) * 0x100000001b3ULL;
			h = (h ^ p[i + 3]) * 0x100000001b3ULL;
		}

		for (int i = len - (len%4); i < len; i++)
		{
			h = (h ^ p[i]) * 0x100000001b3ULL;
		}

		return h;
	}

	Errors readEntireFileWithCheckSum(void* data, size_t size, const char* name)
	{
		std::ifstream f(name, std::ios::binary);
		if (f.is_open())
		{
			f.seekg(0, std::ios_base::end);
			size_t readSize = f.tellg();
			f.seekg(0, std::ios_base::beg);
			
			size_t sizeWithCheckSum = size + sizeof(HashType);

			if (readSize != sizeWithCheckSum)
			{
				return fileSizeDitNotMatch;
			}
			else
			{
				f.read((char*)data, size);
				HashType checkSum = 0;
				f.read((char*)&checkSum, sizeof(HashType));

				auto testCheck = fnv_hash_1a_64(data, size);

				if (testCheck != checkSum)
				{
					return checkSumFailed;
				}
				else
				{
					return noError;
				}
			}
			
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors writeEntireFileWithCheckSum(const void* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);

		if (f.is_open())
		{
			f.write((char*)data, size);
			auto testCheck = fnv_hash_1a_64(data, size);

			f.write((char*)&testCheck, sizeof(testCheck));

			return noError;
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors writeEntireFile(const std::vector<char>& data, const char* name)
	{
		return writeEntireFile(data.data(), data.size(), name);
	}

	Errors writeEntireFile(const void* data, size_t size, const char* name)
	{
		std::ofstream f(name, std::ios::binary);
	
		if (f.is_open())
		{
			f.write((char*)data, size);
			return noError;
		}
		else
		{
			return couldNotOpenFinle;
		}
	}

	Errors safeSave(const void* data, size_t size, const char* nameWithoutExtension, bool reportnotMakingBackupAsAnError)
	{
		std::string file1 = nameWithoutExtension; file1 += "1.bin";
		std::string file2 = nameWithoutExtension; file2 += "2.bin";

		auto err = writeEntireFileWithCheckSum((char*)data, size, file1.c_str());

		if (err == couldNotOpenFinle)
		{
			return couldNotOpenFinle;
		}
		else
		{
			auto err2 = writeEntireFileWithCheckSum((char*)data, size, file2.c_str());	

			if (err2 == couldNotOpenFinle)
			{
				if (reportnotMakingBackupAsAnError)
				{
					return couldNotMakeBackup;
				}
				else
				{
					return noError;
				}
			}
			else
			{
				return noError;
			}
		}
	}

	Errors safeLoad(void* data, size_t size, const char* nameWithoutExtension, bool reportLoadingBackupAsAnError)
	{
		std::string file1 = nameWithoutExtension; file1 += "1.bin";
		std::string file2 = nameWithoutExtension; file2 += "2.bin";

		auto err = readEntireFileWithCheckSum((char*)data, size, file1.c_str());

		if (err == noError)
		{
			return noError;
		}
		else 
		{
			//load backup
			auto err2 = readEntireFileWithCheckSum((char*)data, size, file2.c_str());

			if (err2 == noError)
			{
				if (reportLoadingBackupAsAnError)
				{
					return readBackup;
				}
				else
				{
					return noError;
				}
			}
			else
			{
				return err2;
			}
		}
	}

	Errors safeLoadBackup(void* data, size_t size, const char* nameWithoutExtension)
	{
		std::string file2 = nameWithoutExtension; file2 += "2.bin";

		//load backup
		auto err2 = readEntireFileWithCheckSum((char*)data, size, file2.c_str());
		return err2;
	}

#if defined WIN32 || defined _WIN32 || defined __WIN32__ || defined __NT__

	Errors openFileMapping(FileMapping& fileMapping, const char* name, size_t size, bool createIfNotExisting)
	{
		fileMapping = {};

		DWORD createDisposition = 0;

		if (createIfNotExisting)
		{
			createDisposition = OPEN_ALWAYS;
		}
		else
		{
			createDisposition = OPEN_EXISTING;
		}

		fileMapping.internal.fileHandle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0,
			NULL, createDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

		if (fileMapping.internal.fileHandle == INVALID_HANDLE_VALUE)
		{
			auto err = GetLastError();
			return Errors::couldNotOpenFinle;
		}

		fileMapping.internal.fileMapping = CreateFileMappingA(fileMapping.internal.fileHandle, NULL, PAGE_READWRITE, 0, size, NULL);

		if (fileMapping.internal.fileMapping == NULL)
		{
			CloseHandle(fileMapping.internal.fileHandle);
			return Errors::couldNotOpenFinle;
		}


		fileMapping.pointer = MapViewOfFile(fileMapping.internal.fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, size);

		if (fileMapping.pointer == nullptr)
		{
			CloseHandle(fileMapping.internal.fileMapping);
			CloseHandle(fileMapping.internal.fileHandle);
			return Errors::couldNotOpenFinle;
		}

		fileMapping.size = size;


		return Errors::noError;
	}

	void closeFileMapping(FileMapping& fileMapping)
	{
		UnmapViewOfFile(fileMapping.pointer);
		CloseHandle(fileMapping.internal.fileMapping);
		CloseHandle(fileMapping.internal.fileHandle);
		fileMapping = {};
	}

#elif defined __linux__

	Errors openFileMapping(FileMapping& fileMapping, const char* name, size_t size, bool createIfNotExisting)
	{
		int createDisposition = 0;
		if(createIfNotExisting)
		{
			createDisposition = O_CREAT;
		}

		fileMapping.internal.fd = open(name, O_RDWR | createDisposition);

		if(fileMapping.internal.fd == -1)
		{
			return Errors::couldNotOpenFinle;
		}

		if(ftruncate(fileMapping.internal.fd, size) == -1)
		{
			close(fileMapping.internal.fd);
			return Errors::couldNotOpenFinle;
		}
		
		fileMapping.pointer = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED,
				fileMapping.internal.fd, 0);

		if(fileMapping.pointer == MAP_FAILED)
		{
			fileMapping.pointer = 0;
			close(fileMapping.internal.fd);
			return Errors::couldNotOpenFinle;
		}

		fileMapping.size = size;
		

		return Errors::noError;
	}

	void closeFileMapping(FileMapping& fileMapping)
	{
		fsync(fileMapping.internal.fd);
		msync(fileMapping.pointer, fileMapping.size, MS_SYNC);
		munmap(fileMapping.pointer, fileMapping.size);
		close(fileMapping.internal.fd);
		
		fileMapping = {};
	}

#endif	

}
