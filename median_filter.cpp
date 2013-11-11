#include <vector>
#include <iostream>
#include <algorithm>

#include "Image.h"

typedef unsigned char unit;

void add_neighbourhood(std::vector<unit> &dest,const Image &source, int w, int h, int depth, int shift);

int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " [image_name]\n";
		return -1;
 	}

	Image input;
	Image output;

	input.Load(argv[1]);
	output.Build(input.width, input.height, input.bpp);

	int px_depth = input.bpp == 8 ? 1 : 3;

	std::cout << "Appling median filter to " << argv[1] << std::endl;
	std::vector<unit> tmp_buffer(4);
	std::vector<unit> aux_buffer_1(4);
	std::vector<unit> aux_buffer_2(4);

	for(int i = 0; i < input.height; ++i){
		for(int j = 0; j < input.width; ++j){	
			if(px_depth == 1){
				add_neighbourhood(tmp_buffer, input, j, i, px_depth, 0);
				std::sort(tmp_buffer.begin(), tmp_buffer.end());
				output.buffer[j + i*input.width] = tmp_buffer[4];
				tmp_buffer.clear();
			}
			else{
				add_neighbourhood(tmp_buffer, input, j, i, px_depth, 0);
				std::sort(tmp_buffer.begin(), tmp_buffer.end());
				output.buffer[j*px_depth + i*input.width*px_depth] = tmp_buffer[4];
				tmp_buffer.clear();

				add_neighbourhood(aux_buffer_1, input, j, i, px_depth, 1);  
				std::sort(aux_buffer_1.begin(), aux_buffer_1.end());
				output.buffer[j*px_depth + 1 + i*input.width*px_depth] = aux_buffer_1[4];
				aux_buffer_1.clear();

				add_neighbourhood(aux_buffer_2, input, j, i, px_depth, 2);  
				std::sort(aux_buffer_2.begin(), aux_buffer_2.end());
				output.buffer[j*px_depth + 2 + i*input.width*px_depth] = aux_buffer_2[4];
				aux_buffer_2.clear();
			}
		}
	}

	if(output.bpp == 8)
		output.Save("median_filter.pgm");
	else
		output.Save("median_filter.ppm");

	return 0;
}

void add_neighbourhood(std::vector<unit> &dest,const Image &source, int w, int h, int depth, int shift){
	int size = 3;
	for(int i = h-1; i < (h-1) + size; ++i){
		for(int j = w-1; j < (w-1) + size; ++j){
			if( j > source.width || j < 0 || i > source.height || i < 0)
				dest.push_back(0);
			else
				dest.push_back(source.buffer[j*depth + shift + i*source.width*depth]);
		}
	}
}
