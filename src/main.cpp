#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>

#include <physfs.h>

#include "main.h"


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