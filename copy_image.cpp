#include <iostream>

#include "Image.h"


int main(int argc, char** argv){
	Image input;
	Image output;

	input.Load("input_image.pgm");
	output.Build(input.width, input.height, input.bpp);

	// scanning image from left to right row by row
	for(int i = 0; i < input.height; ++i)
		for(int j = 0; j < input.width; ++j)
			output.buffer[j + i*input.width] = input.buffer[j + i*input.width];

	output.Save("copy_image_result.pgm");
	std::cout << "Copy completed\n";
}
