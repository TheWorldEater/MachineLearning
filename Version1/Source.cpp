#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>
#include <glad.h>
#include <glfw3.h>

BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoheader;

unsigned char* shading;

int error = 0;
void errorwrite() {
	switch (error){
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
	default:
		std::cout << "Some other Error/Error with int error!" << std::endl;
		break;
	}
}

int round_up_to(int val, int round_to) {
	val += round_to - 1;
	return val - (val % round_to);
}

void shador(FILE* pic) {
	int row_size = round_up_to(infoheader.biWidth * 3, 4);
	int pixels_size = row_size * infoheader.biHeight;

	unsigned char* src = (unsigned char*)malloc(pixels_size);

	fseek(pic, fileheader.bfOffBits, SEEK_SET);
	auto read = fread(src, 1,pixels_size, pic);
	if (read != pixels_size) {
		error = 2;
		return;
	}

	for (int y = 0;y<infoheader.biHeight;y++) {
		for (int x = 0;x<infoheader.biWidth;x++) {
			unsigned char* bgr = &src[ y * row_size + x * 3 ];


			shading[y * infoheader.biWidth + x] = (bgr[0] + bgr[1] + bgr[2]) / 3;
		}
	}
	free(src);
}

void readbmp() {
	FILE* pic = fopen("dabs/test.bmp", "rb");
	if (pic == 0) {
		error = 1;
		return;
	}
	
	//reads first header
	int temp = (int)fread(&fileheader, 1, sizeof(fileheader), pic);
	if (temp != sizeof(fileheader)) {
		error = 2;
		return;
	}
	if (memcmp(&fileheader.bfType, "BM", 2) != 0) {
		error = 3;
		return;
	}
	

	//reads second header
	temp = (int)fread(&infoheader, 1, sizeof(infoheader), pic);
	if (temp != sizeof(infoheader)) {
		error = 2;
		return;
	}
	//Checks bit_per_pix
	if (infoheader.biBitCount == 0 || infoheader.biHeight < 0) {
		error = 4;
		return;
	}
	//checks for Compression
	if (infoheader.biCompression != BI_RGB) {
		error = 5;
		return;
	}

	shading = (unsigned char*)malloc(infoheader.biHeight * infoheader.biWidth * sizeof(char));
	shador(pic);
	if (error != 0) {
		return;
	}

}


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