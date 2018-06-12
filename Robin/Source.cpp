#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include <stdio.h>


#include <cmath>

#include "../imgl/imgl.hpp"
using imgl::u8;
using imgl::u64;
using imgl::f32;
typedef imgl::s32v2 iv2;
typedef imgl::fv2 v2;
using imgl::rgba8;
using imgl::rgb8;
using imgl::rgbf;

#include <memory>
using std::unique_ptr;
using std::make_unique;


std::vector<unsigned char> shading;
#include "Shador.hpp"
#include "Values.h"

void draw () {
	
	if (0) {
		u8 pixels[6][6];
		for (int y=0; y<6; ++y) {
			for (int x=0; x<6; ++x) {
				pixels[y][x] = y == 0 ? 255 : 20;
			}
		}
		
		iv2 sz = iv2(6,6);
		imgl::image_rect(&pixels[0][0], sz, imgl::get_current_region().fit_aspect((v2)sz), true, true);
	} else {
		iv2 sz = iv2(infoheader.biWidth,infoheader.biHeight);
		auto copy = make_unique<rgba8[]>(sz.x * sz.y);
		
		for (int y=0; y<sz.y; ++y) {
			for (int x=0; x<sz.x; ++x) {
				copy[y * sz.x +x] = rgba8(rgb8(shading[y * sz.x +x]), 255);
			}
		}
		
		imgl::image_rect(copy.get(), sz, imgl::get_current_region().fit_aspect((v2)sz), false);
	}
}

int main() {

	readbmp();

	u64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	u64 begin;
	QueryPerformanceCounter((LARGE_INTEGER*)&begin);

	if (0) {
		for (int i = 0; i < infoheader.biHeight*infoheader.biWidth; i++) {
			printf("%03d ", i);//shading.at(i));
			//std::cout << i << " ";//(int)shading.at(i) << " ";

			u64 now;
			QueryPerformanceCounter((LARGE_INTEGER*)&now);
			if ( ((f32)(now -begin) / (f32)freq) >= 4)
				break;
		}
	}

	imgl::window_loop("DAB on 'em haters!", draw, 0, imgl::POLL_FOR_INPUT);

	if (error != 0) {
		errorwrite();
	}

	if (0) {
		char stop;
		std::cin >> stop;
	}
}
