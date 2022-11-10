#include "FileManager.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <physfs.h>

namespace Phoenix
{
	FileManager::FileManager()
	{
		clear();
		m_workingWithDataFolder = true;
		m_fileCache = false;
	}
	FileManager::~FileManager()
	{
		deinit();
		clear();
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
	SP_File FileManager::loadFile(const std::string_view filePath)
	{
		SP_File p_file;

		for (auto& pFile : file) {
			if (pFile->m_filePath.compare(filePath.data()) == 0)
				p_file = pFile;
		}

		if (p_file == nullptr) { // If the file has not been found, we need to load it for the first time
			SP_File new_file = std::make_shared<File>();
			if (new_file->load(filePath, m_workingWithDataFolder, m_password)) {
				file.push_back(new_file);
				m_mem += new_file->m_fileSize;
				p_file = new_file;
				printf("File lodaded ok!: %s\n", new_file->m_filePath.c_str());
			}
			else {
				printf("File not loaded: %s\n", filePath.data());
			}
		}
		else { // If the file is in cache we should not do anything, unless we have been told to upload it again
			if (!m_fileCache) {
				m_mem -= p_file->m_fileSize; // Decrease the overall memory
				if (p_file->load(filePath, m_workingWithDataFolder, m_password)) {
					m_mem += p_file->m_fileSize;
					printf("File force lodaded ok!: %s\n", p_file->m_filePath.c_str());
				}
				else {
					printf("File not loaded: %s\n", filePath.data());
				}
			}
			else {
				printf("File already in memory: %s\n", filePath.data());
			}

		}

		return p_file;
	}

	void FileManager::clear()
	{
		file.clear();
		m_mem = 0;
	}

	File::File()
		:
		m_filePath(""),
		m_fileData(nullptr),
		m_fileSize(0)
	{
	}

	File::~File()
	{
		if (m_fileData)
			free(m_fileData);
		m_fileData = nullptr;
	}

	bool File::load(const std::string_view filePath, bool WorkingWithDataFolder, const std::string Password)
	{
		// Init file data
		if (m_fileData != nullptr)
			free(m_fileData);
		m_filePath = filePath;
		m_fileSize = 0;

		if (WorkingWithDataFolder) {
			// Working with files
			// 
			// Open the stream to 'lock' the file.
			std::ifstream f(filePath.data(), std::ios::in | std::ios::binary);
			m_fileSize = static_cast<uint32_t>(std::filesystem::file_size(filePath.data()));

			// Allocate storage for the buffer
			m_fileData = new char[m_fileSize + 1];

			// Read the whole file into the buffer.
			f.read(m_fileData, m_fileSize);
			m_fileData[m_fileSize] = '\0'; // add the terminator
			f.close();

			return true;
		}
		else {
			std::string filePathWithPassword = filePath.data();

			if (!Password.empty())
				filePathWithPassword += std::string("$") + Password;

			PHYSFS_file* file = PHYSFS_openRead(filePathWithPassword.c_str());
			if (file == NULL) {
				return false;
			}

			m_fileSize = static_cast<int>(PHYSFS_fileLength(file));

			m_fileData = new char[m_fileSize + 1];
			PHYSFS_read(file, m_fileData, 1, static_cast<PHYSFS_uint32>(m_fileSize));
			m_fileData[m_fileSize] = '\0'; // add the terminator
			PHYSFS_close(file);
			return true;
		}
	}


}
