#include <iostream>
#include <cstdlib>

#include "Image.h"


int main(int argc, char** argv){
	Image input;
	Image output;
	
	if(argc < 3){
		std::cout << "Usage: " << argv[0] << " [image] [threshold_value]\n";
		return -1;
	}
	

	input.Load(argv[1]);
	output.Build(input.width, input.height, 8);
	
	int pixel_shift = input.bpp == 8 ? 1 : 3;
	
	std::cout << "Appling threashold ...\n";

	int th = atoi(argv[2]);
	int res = 0;

	for(int i = 0; i < input.height ; ++i ){
		for(int j = 0; j < input.width ; ++j){
			res += input.buffer[j*pixel_shift   +i*input.width*pixel_shift];
			res += input.buffer[j*pixel_shift+1 +i*input.width*pixel_shift];
			res += input.buffer[j*pixel_shift+2 +i*input.width*pixel_shift];
			res = res / 3.0;
		 	res = res > 255 ? 255 : res;
			if(res > th)
				output.buffer[j + i*input.width] = 255;
			else 
				output.buffer[j + i*input.width] = 0;
			
		}
		res = 0;
	}

	std::cout << "Saving result with name 'binarization.pgm'\n";

	output.Save("binarization.pgm");
	return 0;
}
