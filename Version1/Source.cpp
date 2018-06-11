#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <glfw3.h>

#include "Shador.hpp"
#include "Neurons.hpp"



int main() {

	readbmp();

	for (int i = 0; i < infoheader.biHeight*infoheader.biWidth; i++) {
		printf("%03d ", (int)shading[i]);
	}








	if (error != 0) {
		errorwrite();
	}
	free(shading);
	char stop;
	std::cin >> stop;
}