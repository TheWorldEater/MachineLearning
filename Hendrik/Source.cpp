#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>

int error = 0;
std::vector<int> shading;
#include "Shador.hpp"
#include "Values.h"

void errorwrite() {
	switch (error) {
	case 1:
		std::cout << "fopen failed!" << std::endl;
		break;
	case 2:
		std::cout << "fread failed!" << std::endl;
		break;
	case 3:
		std::cout << "Not a BMP!" << std::endl;
		break;
	case 4:
		std::cout << "Not supported Filetype!" << std::endl;
		break;
	case 5:
		std::cout << "Some kind of supression" << std::endl;
		break;
	case 6:
		std::cout << "At least two Outputs are needed" << std::endl;
	default:
		std::cout << "Some other Error/Error with int error!" << std::endl;
		break;
	}
}

int main() {

	readbmp();

	for (int i = 0; i < infoheader.biHeight*infoheader.biWidth; i++) {
		printf("%03d ", shading.at(i));
	}








	if (error != 0) {
		errorwrite();
	}
	char stop;
	std::cin >> stop;
}