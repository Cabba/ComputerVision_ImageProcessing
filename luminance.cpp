#include <iostream>
#include <cstdlib>
#include "Image.h"

int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " [new_luminance_value]\n";
		return -1;
	}

	Image input;
	Image output;

	input.Load("input_image.pgm");
	output.Build(input.width, input.height, input.bpp);

	int reduction = atoi(argv[1]);
	int tmp;

	for(int i = 0; i < input.height; ++i)
		for(int j = 0; j < input.width; ++j){
			tmp = input.buffer[j + input.width*i] - reduction;		
			output.buffer[j + input.width*i] = tmp < 0 ? 0 : tmp;
		}

	std::cout << "Saving the input ...\n";

	output.Save("luminance_reduction.pgm");

	return 0;
}
