#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <physfs.h>

#include "main.h"



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
	/*#elif defined(EMSCRIPTEN_TARGET)
		std::ifstream file(_filename, std::ios::in|std::ios::binary|std::ios::ate);
		assert(file.is_open());

		*_size = file.tellg();
		assert(*_size != 0);

		natU8* buffer = new natU8[*_size];
		file.seekg (0, std::ios::beg);
		file.read ((char*)buffer, *_size);
		file.close();

		return buffer;
	#else

	#endif*/
}


int main(int argc, char* argv[]) {

	float time = 0;

	char c;
	std::cout << "Press ESC to exit!" << std::endl;

	PHYSFS_init(argv[0]);

	PHYSFS_Version compiled;
	PHYSFS_VERSION(&compiled);
	printf("PhysFS version %d.%d.%d ...\n",	compiled.major, compiled.minor, compiled.patch);

	if (0 == PHYSFS_mount("files//data.zip", NULL, 0))
		printf("Error opening file, error: %s\n", PHYSFS_getLastError());

	/*
	PHYSFS_file* file = PHYSFS_openRead("test.txt");
	if (!file) {
		printf("Error opening file inside zip, error: %s\n", PHYSFS_getLastError());
		return 0;
	}
		
	PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
	printf("test.txt size: %d\n", (unsigned int)file_size);
	*/
	

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

	std::cout << "Exit!" << std::endl;
}