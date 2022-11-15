#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace Phoenix
{
	// Class File
	class File;
	using SP_File = std::shared_ptr<File>;

	class File final {
	public:
		std::string	m_filePath;	// Path used to avoid uploading the same file twice
		char*		m_fileData;	// File data in memory
		uint32_t	m_fileSize;	// File size in bytes

	public:
		File();
		virtual ~File();

		bool load(const std::string_view filePath, bool WorkingWithDataFolder, const std::string Password = "");
	};

	// Class File Manager
	class FileManager final {

	public:
		FileManager();
		~FileManager();

	public:

		void init(const char* argv0);	// Init PhysFS
		void deinit();					// Deinit PysFS and unmount any file
		const std::string getVersion() { return m_physfsVersion; };

		bool mountData(const std::string_view dataFilePath);	// Mount a Data file or folder
		void setPassword(const std::string_view password) { m_password = password; };		// Set a password for files
		void clearPassword() { m_password = ""; };

		void setCache(bool cacheEnabled) { m_fileCache = cacheEnabled; };
		SP_File loadFile(const std::string_view filePath);		// Load a file to memory
		void clear();	// delete all files from memory

		
	private:
		bool m_workingWithDataFolder;
		bool m_fileCache;				// If cache is disabled, file data will be refreshed even if the file exists

		std::string m_password;
		std::string m_physfsVersion;
	
	public:
		std::vector<SP_File>	m_files;	// file list
		uint32_t				m_mem = 0;	// Memory in bytes loaded to memory

	};
}
