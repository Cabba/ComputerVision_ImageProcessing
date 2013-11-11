#include <iostream>
#include "Image.h"

int main(int argc, char** argv){
	if(argc < 3){
		std::cout << "Usage: " << argv[0] << " [first_image] [second_image]\n";
		return -1;
	}

	Image main_image, sup_image, output;
	
	main_image.Load(argv[1]);
	sup_image.Load(argv[2]);
	if( main_image.width != sup_image.width || main_image.height != sup_image.height){
		std::cout << "The two images are not compatible!\n";
		return -1;
	}
	output.Build(main_image.width, main_image.height, main_image.bpp);
	
	int width = main_image.width;
	int height = main_image.height;
	int result = 0;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			result = main_image.buffer[j + i*width] - sup_image.buffer[j +i*width];
			result = result < 0 ? 0 : result;
			result = result > 255 ? 255 : result;
			output.buffer[j + i*width] = result;
		}
	}

	output.Save("difference.pgm");
	
	return 0;
}
