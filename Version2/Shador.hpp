#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>
#include <Windows.h>

BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoheader;





int round_up_to(int val, int round_to) {
	val += round_to - 1;
	return val - (val % round_to);
}

void shador(FILE* pic) {
	int row_size = round_up_to(infoheader.biWidth * 3, 4);
	int pixels_size = row_size * infoheader.biHeight;

	unsigned char* src = (unsigned char*)malloc(pixels_size);

	fseek(pic, fileheader.bfOffBits, SEEK_SET);
	auto read = fread(src, 1, pixels_size, pic);
	if (read != pixels_size) {
		error = 2;
		return;
	}

	for (int y = 0; y<infoheader.biHeight; y++) {
		for (int x = 0; x<infoheader.biWidth; x++) {
			unsigned char* bgr = &src[y * row_size + x * 3];


			shading.push_back((bgr[0] + bgr[1] + bgr[2]) / 3);
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
	//Checks bit_per_pix and that hight is positive(bottom up)
	if (infoheader.biBitCount == 0 || infoheader.biHeight < 0) {
		error = 4;
		return;
	}
	//checks for Compression
	if (infoheader.biCompression != BI_RGB) {
		error = 5;
		return;
	}

	shador(pic);
	if (error != 0) {
		return;
	}

}
