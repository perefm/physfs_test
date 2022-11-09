#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Phoenix
{

	class FileManager final {

	public:
		~FileManager();

	public:

		void init(const char* argv0);	// Init PhysFS
		void deinit();					// Deinit PysFS and unmount any file

		bool mountData(const std::string_view dataFilePath);	// Mount a Data file or folder

		bool loadFileToMem(const std::string_view filePath, char* &fileData, uint32_t &fileSize);		// Load a file to memory

		const std::string getVersion() { return m_physfsVersion; };

	private:
		int m_workingWithDataFolder = true;
		std::string m_physfsVersion;

	};
}
