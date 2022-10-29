#include <conio.h>
#include <iostream>
#include <string>
#include <fstream>

#include "main.h"


int main() {

	float time = 0;

	char c;
	std::cout << "Press ESC to exit!" << std::endl;


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

	std::cout << "Exit!" << std::endl;
}