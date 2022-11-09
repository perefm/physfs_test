#include "FileManager.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <physfs.h>

namespace Phoenix
{
	FileManager::~FileManager()
	{
		deinit();
	}
	void FileManager::init(const char* argv0)
	{
		static const char* mainArgv0 = argv0;
		deinit();
		PHYSFS_init(mainArgv0);

		// Update version
		PHYSFS_Version compiled;
		PHYSFS_VERSION(&compiled);

		m_physfsVersion = std::to_string(compiled.major) + "." + std::to_string(compiled.minor) + "." + std::to_string(compiled.patch);
	}

	void FileManager::deinit()
	{
		if (PHYSFS_isInit() != 0)
		{
			PHYSFS_deinit();
		}
	}

	bool FileManager::mountData(const std::string_view dataFilePath)
	{
		if (std::filesystem::is_directory(dataFilePath.data()))
		{
			m_workingWithDataFolder = true;
			return true;
		}
		else {
			if (0 == PHYSFS_mount((const char*)dataFilePath.data(), NULL, 0))
				return false;
			m_workingWithDataFolder = false;
		}
		return true;
	}

	void FileManager::setPassword(const std::string_view password)
	{
		m_password = password;
	}

	bool FileManager::loadFileToMem(const std::string_view filePath, char* &fileData, uint32_t &fileSize)
	{
		fileSize = 0;

		if (m_workingWithDataFolder) {
			// Working with files
			// 
			// Open the stream to 'lock' the file.
			std::ifstream f(filePath.data(), std::ios::in | std::ios::binary);
			fileSize = static_cast<uint32_t>(std::filesystem::file_size(filePath.data()));

			// Allocate storage for the buffer
			fileData = new char[fileSize+1];

			// Read the whole file into the buffer.
			f.read(fileData, fileSize);
			fileData[fileSize] = '\0'; // add the terminator
			f.close();

			return true;
		}
		else {
			std::string filePathWithPassword = filePath.data();
			
			if (!m_password.empty())
				filePathWithPassword += std::string("$") + m_password;

			PHYSFS_file* file = PHYSFS_openRead(filePathWithPassword.c_str());
			if (file == NULL) {
				return false;
			}

			fileSize = static_cast<int>(PHYSFS_fileLength(file));

			fileData = new char[fileSize+1];
			PHYSFS_read(file, fileData, 1, static_cast<PHYSFS_uint32>(fileSize));
			fileData[fileSize] = '\0'; // add the terminator
			PHYSFS_close(file);
			return true;
		}
	}	
}
