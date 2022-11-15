#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <physfs.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
	printf("File content:\n%s\n\n", buffer);
}

struct sImage {
	unsigned char* imageData;
	int imageWidth;
	int imageHeight;
	int imageComponents;
};

int main(int argc, char* argv[]) {

	std::cout << "Press ESC to exit!" << std::endl;

	FileManager fileManager;
	sImage image;

	printf("FILEMANAGER TESTS\n");
	SP_File myfile;
	fileManager.init(argv[0]);
	fileManager.setCache(true);
	if (fileManager.mountData("test_files/data_pass123.zip")) {
		fileManager.setPassword("123");
		fileManager.loadFile("test.txt");
		myfile = fileManager.loadFile("images/tv.jpg");
		if (myfile) {
			image.imageWidth = image.imageHeight = image.imageComponents = 0;
			image.imageData = stbi_load_from_memory((unsigned char*)(myfile->m_fileData), myfile->m_fileSize, &image.imageWidth, &image.imageHeight, &image.imageComponents, 0);
			printf("Image Size: %dx%d - Channels: %d\n", image.imageWidth, image.imageHeight, image.imageComponents);
			stbi_image_free(image.imageData);
		}
		myfile = fileManager.loadFile("images/tv.jpg");
		if (myfile) {
			image.imageWidth = image.imageHeight = image.imageComponents = 0;
			image.imageData = stbi_load_from_memory((unsigned char*)(myfile->m_fileData), myfile->m_fileSize, &image.imageWidth, &image.imageHeight, &image.imageComponents, 0);
			printf("Image Size: %dx%d - Channels: %d\n", image.imageWidth, image.imageHeight, image.imageComponents);
			stbi_image_free(image.imageData);
		}
		fileManager.loadFile("folder_one/test one.txt");
		fileManager.loadFile("this should fail.txt");

		printf("Memory: %d\n", fileManager.m_mem);
		for (auto &pFile: fileManager.m_files) {
			printf("Stored file: %s, size: %d\n", pFile->m_filePath.c_str(), pFile->m_fileSize);
		}

	}

	fileManager.clear();
	printf("Cleared fileManager...\n");
	printf("Memory: %d\n", fileManager.m_mem);
	for (auto& pFile : fileManager.m_files) {
		printf("Stored file: %s, size: %d\n", pFile->m_filePath.c_str(), pFile->m_fileSize);
	}


	fileManager.deinit();
	
	std::cout << "Exit!" << std::endl;
}


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