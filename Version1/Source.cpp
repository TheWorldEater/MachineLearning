#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <glfw3.h>

#include "Shador.cpp"



int main() {

	readbmp();

	for (int i = 0; i < infoheader.biHeight*infoheader.biWidth; i++) {
		//std::cout << (int)shading[i] << " ";
		printf("%03d ", (int)shading[i]);
	}








	if (error != 0) {
		errorwrite();
	}
	free(shading);
	char stop;
	std::cin >> stop;
}