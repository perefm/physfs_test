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
	printf("test.txt size: %d\n", fileSize);

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


int main() {

	float time = 0;

	char c;
	std::cout << "Press ESC to exit!" << std::endl;

	PHYSFS_init(NULL);

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
	int file_size = 0;
	std::string file = "test.txt";
	char* buffer = readFile(file, file_size);

	printf("File content: %s\n", buffer);


	while (true)
	{
		c = _getch();
		
		if (c == 27)
			break;
		else if (c == 'e') { // SHOW ENTITIES
			
		}
		else if (c == 'l') { // LOAD FILE
			
		}

		else if (c == 'p') { // RUN SOME MASSIVE SAVE & LOADING
		
		}

		else {

		}
	}

	PHYSFS_deinit();

	std::cout << "Exit!" << std::endl;
}