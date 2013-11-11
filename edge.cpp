#include <vector>
#include <iostream>
#include <algorithm>

#include "Image.h"

int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " [input_name]\n";
		return -1;
 	}

	Image input;
	Image output;

	input.Load(argv[1]);
	std::cout << "Loading " << argv[1] << " dimension: " << input.width << "x" << input.height << std::endl;
	output.Build(input.width, input.height, input.bpp);

	int result = 0;
	for(int i = 0; i < input.height; ++i){
		for(int j = 0; j < input.width; ++j){
			// Out of bounds conditions
			if(j - 1 < 0 || j + 1 > input.width) 
				output.buffer[j + i*input.width] = 0;

			else if(i - 1 < 0 || i + 1 > input.height)
				output.buffer[j + i*input.width] = 0;

			else{
				result += 4 * input.buffer[j + i*input.width];
				result += -1 * input.buffer[j - 1 + i*input.width];
				result += -1 * input.buffer[j + 1 + i*input.width];
				result += -1 * input.buffer[j + (i-1)*input.width];
				result += -1 * input.buffer[j + (i+1)*input.width];
				std::cout << result << std::endl;
				output.buffer[j + i*input.width] = result < 0 ? 0 : result;

			}
			result = 0;
		}
	}

	output.Save("edge.pgm");
	
	return 0;
}
