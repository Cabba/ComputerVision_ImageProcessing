#include <iostream>
#include <cstdlib>

#include "Image.h"

int main(int argc, char** argv){
	if( argc < 6){
		std::cout << "Usage: " << argv[0] << " [input] [start_x] [start_y] [cut_width] [cut_height]\n";
		return -1;
	}
	
	int cut_w = atoi(argv[4]);
	int cut_h = atoi(argv[5]);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	Image input, output;
	input.Load(argv[1]);
	output.Build(cut_w, cut_h, input.bpp);

	std::cout << cut_w << " " << cut_h << std::endl;
	for(int i = 0; i < cut_h; ++i){
		for(int j = 0; j < cut_w; ++j){
			output.buffer[j + i*cut_w] = input.buffer[(j+x) +(i+y)*input.width];
		}
	}

	output.Save("cut.pgm");

	return 0;
}
