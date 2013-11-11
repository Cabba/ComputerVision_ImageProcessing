#include <iostream>
#include <fstream>

#include "Image.h"

const int histogram_dim = 255;

int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " [image]\n";
		return -1;
	}
	Image input;
	input.Load(argv[1]);

	// Setting the pixel shift for black and withe and color images 
	int px_sft = input.bpp == 8 ? 1 : 3;
	int total_h_dim = histogram_dim * px_sft + (input.bpp != 8 ? 2: 0);

	std::cout << "Histogram dimension: " << total_h_dim << std::endl;
	// Declaring and initializing the istogram
	int histogram[total_h_dim];
	for(int i = 0; i < total_h_dim; ++i)
		histogram[i] = 0;

	std::cout << "Calculating istrogram ..\n";

	for(int i = 0; i < input.height; ++i){
		for(int j = 0; j < input.width; ++j){
			histogram[input.buffer[j*px_sft + i*input.width*px_sft]*3] += 1;
			histogram[input.buffer[j*px_sft +1 + i*input.width*px_sft]*3 + 1 ] +=1;
			histogram[input.buffer[j*px_sft +2 + i*input.width*px_sft]*3 + 2 ] +=1;
			//	std::cout << j+2 + i*input.width*px_sft << std::endl;
		}
	}
	
	std::cout << "Writing istogram to file ...\n";
	std::cout << "plot the result with <plot 'bw_histogram.dat' using 1:2 w l>\n";	
	// Print the result
	std::ofstream output;
	output.open("histogram_data.dat");
	for(int i = 0; i < histogram_dim; ++i)
		if(px_sft == 1)
			output << i << " " << histogram[i] << std::endl;
		else if(px_sft == 3){
			output << i << " " << histogram[i*3] 
					<< " " << histogram[i*3 + 1]
					<< " " << histogram[i*3 + 2] 
					<< std::endl;
		}
	
	output.close();

	return 0;
}
