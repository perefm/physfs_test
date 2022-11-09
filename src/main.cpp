#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <physfs.h>

#include "main.h"

#include "Utils/FileManager.h"

using namespace Phoenix;

char* readFile(std::string& fileName, int& fileSize)
{
	PHYSFS_file* file = PHYSFS_openRead(fileName.c_str());
	if (file == NULL) {
		printf("Error opening file %s, error: %s\n", fileName.c_str(), PHYSFS_getLastError());
		return NULL;
	}

	fileSize = static_cast<int>(PHYSFS_fileLength(file));

	char* buffer = new char[fileSize];
	PHYSFS_read(file, buffer, 1, static_cast<PHYSFS_uint32>(fileSize));
	PHYSFS_close(file);

	return buffer;
}

void printBuffer(std::string fileName, char* buffer, int buffersize) {
	printf("File name: %s\n", fileName.c_str());
	printf("File size: %d\n", buffersize);
	printf("File content:\n%s\n", buffer);
	printf("-----------\n");
}

int main(int argc, char* argv[]) {

	std::cout << "Press ESC to exit!" << std::endl;

	FileManager file;

	// Init PhysFS and mount a FOLDER file
	file.init(argv[0]);
	printf("PhysFS version: %s\n", file.getVersion().c_str());
		
	file.mountData("test_files"); // Mount a folder
	char* fileData = nullptr;
	uint32_t fileSize;
	bool test = file.loadFileToMem("test_files/kaka.txt", fileData, fileSize); // Open a file from folder
	printBuffer("[FOLDER] test_files/kaka.txt", fileData, fileSize);
	
	file.deinit();
	delete[] fileData;
	fileSize = 0;

	// Init PhysFS and mount a ZIP file
	file.init(argv[0]);
	file.mountData("test_files/data.zip");
	file.loadFileToMem("test.txt", fileData, fileSize);
	printBuffer("[FILE] test.txt", fileData, fileSize);

	delete[] fileData;
	fileSize = 0;
	file.loadFileToMem("folder two/test.txt", fileData, fileSize);
	printBuffer("[FILE] folder two/test.txt", fileData, fileSize);

	delete[] fileData;
	fileSize = 0;
	file.deinit();

	/*
	// Test: Open a zip file and show the content
	char c;
	PHYSFS_init(argv[0]);

	PHYSFS_Version compiled;
	PHYSFS_VERSION(&compiled);
	printf("PhysFS version %d.%d.%d ...\n",	compiled.major, compiled.minor, compiled.patch);

	if (0 == PHYSFS_mount("test_files//data.zip", NULL, 0))
		printf("Error opening file, error: %s\n", PHYSFS_getLastError());

	while (true)
	{
		c = _getch();
		
		if (c == 27)
			break;
		else if (c == 'l') { // Load all files in base dir
			std::string filePath = "";
			char** paths = PHYSFS_enumerateFiles(filePath.c_str());
			char** path;
			for (path = paths; *path != nullptr; path++)
			{
				PHYSFS_Stat fileStat;
				std::string fullPath = filePath + '/' + *path;
				if (PHYSFS_stat(fullPath.c_str(), &fileStat) == 0)
				{
				}
				else if (fileStat.filetype == PHYSFS_FILETYPE_DIRECTORY)
				{
					printf("Found folder: [%s].\n", fullPath.c_str());
				}
				else {
					printf("Found file: [%s].\n", fullPath.c_str());
					
					// ReadFile					
					int file_size = 0;
					char* buffer = readFile(fullPath, file_size);
					printf("File size: %d\n", file_size);
					printf("File content: %s\n", buffer);

				}
			}
			PHYSFS_freeList(paths);
		}
	}

	PHYSFS_deinit();

	*/
	std::cout << "Exit!" << std::endl;
}