#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

#include "Image.h"

static const int gc_th = 200;

struct Vector2i{
	int x, y;
	Vector2i(){ x = 0; y = 0; }
	Vector2i(int x, int y){ this->x = x; this->y = y; }
	void set(int x, int y){ this->x = x; this->y = y; } 
};


class CBlob{
private:
	std::vector<Vector2i> m_points;

public:
	CBlob(){
	}

	std::vector<Vector2i>& get_points(){
		return m_points;
	}

	void add_point(int x, int y){
		Vector2i v(x, y);
		m_points.push_back(v);
	}

};

CBlob* generate_blob(Image& input, int x, int y, bool *evaluation_map){
	std::vector<Vector2i> to_eval;
	CBlob* blob = new CBlob;
	Vector2i tmp_vect;

	blob->add_point(x, y); 
	evaluation_map[x +y*input.width] = true;
	
	for(int i = y - 1; i < y + 2; ++i){
		for(int j = x - 1; j < x + 2; ++j){
			if( i < input.height && j  < input.width && input.buffer[j + i*input.width] > gc_th ){
				tmp_vect.set(j, i);
				to_eval.push_back( tmp_vect ); 
			}
		}
	}

	while(!to_eval.empty()){
		Vector2i v = to_eval.back();
		blob->add_point(v.x, v.y);
		to_eval.pop_back();

		for( int i = v.y-1; i < v.y + 2; ++i){
			for(int j = v.x-1; j < v.x + 2; ++j){
				if( 	j >= 0 && i >= 0 
					&& j < input.width 
					&& i < input.height 
					&& input.buffer[j + i*input.width] > gc_th
					&& evaluation_map[j + i*input.width] == false
				  ){
					evaluation_map[j+i*input.width] = true;
					tmp_vect.set(j, i);
					to_eval.push_back( tmp_vect );
				}
			} 
		}
	}
	return blob;

}

int main(int argc, char** argv){
	if(argc < 2){
		std::cout << "Usage: " << argv[0] << " [pgm_image]\n";
		return -1;
	}
	
	Image input;
	Image output;

	input.Load(argv[1]);
	if(input.bpp != 8){
		std::cout << "The algorithm require a ppm image\n";
		return -1;
	}
	
	int th = 100;

	std::vector<CBlob*> blobs;

	// Initializing the array for check if a pixel was already evaluated
	bool *is_eval = new bool[input.width*input.height];
	for(int k = 0; k < input.width*input.height; ++k)
		is_eval[k] = false;

	// Looping for all the pixels
	for(int i = 0; i < input.height; ++i){
		for(int j = 0; j < input.width; ++j){
			if(input.buffer[j + i*input.width] > gc_th && is_eval[j + i*input.width] == false ){
				CBlob *blob = generate_blob(input,j, i, is_eval) ;
				blobs.push_back( blob );
			}		
		}
	}

	std::cout << "Generated " << blobs.size() << " blobs\n";

	// Saving the output to image
	output.Build(input.width, input.height, 24);

	srand(time(NULL));
	int r, g, b;
	for(int k = 0; k < output.width*output.height*3; ++k)
		output.buffer[k] = 0;

	for(int i = 0; i < blobs.size(); ++i){	
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
		std::vector<Vector2i> tmp = blobs[i]->get_points();
		for(int j = 0; j < tmp.size(); ++j){
			output.buffer[tmp[j].x*3 + tmp[j].y*input.width*3] = r;
			output.buffer[tmp[j].x*3 + 1 + tmp[j].y*input.width*3] = g;
			output.buffer[tmp[j].x*3 + 2 + tmp[j].y*input.width*3] = b;
			
		}
	}
	
	output.Save("labelling.pgm");
	
	// Freeing the memory
	delete is_eval;
	while(!blobs.empty()){
		delete blobs.back();
		blobs.pop_back();
	}

	return 0;
}
