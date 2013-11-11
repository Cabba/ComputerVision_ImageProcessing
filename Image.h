/*#####################################
  Corso di visione artificiale
  Elaborazione di immagini in C++
  Gestione immagini pnm
#####################################*/

#ifndef _IMAGE_H
#define _IMAGE_H

#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////

/// a simple image structure
struct Image {
  /// geometry
  unsigned int width, height;
  /// bit per pixel (8 is greyscale, 24 is rgb color)
  unsigned int bpp;
  
  /// the bitmap
  unsigned char *buffer;
  
  
public:

  Image();
  ~Image();
  
  /// Allocate the inner buffer
  /// @param width,height the image geometry
  /// @param bpp Bit Per Pixel (8 grey, 24 color rgb)
  void Build(unsigned int width, unsigned int height, unsigned int bpp);
  
  /// Save the image on @a filename
  bool Save(const char *filename) const;
  
  /// Load the image from @a filename 
  ///  and initialize with,height,bpp and buffer field of struct
  bool Load(const char *filename);
  
};


///////////////////////////////////////////////////////////////////////////////////////////

Image::Image() : width(0), height(0), bpp(0), buffer(0) { }
Image::~Image() { delete [] buffer; }

void Image::Build(unsigned int width, unsigned int height, unsigned int bpp)
{
  delete [] buffer;
  
  this->width = width;
  this->height = height;
  this->bpp = bpp;
  
  if(bpp == 24)
  {
    buffer = new unsigned char [width * height * 3];
  }
  else
  if(bpp == 8)
  {
    buffer = new unsigned char [width * height];
  }
  else
  {
    std::cerr << "Invalid bpp = " << bpp << std::endl;
  }
}

bool Image::Save(const char *filename) const
  {
    std::ofstream out(filename, std::ios::out | std::ios::binary);
    if(out)
      {
      if(bpp == 24)
      {
      out << "P6\n" << width << ' ' << height << "\n255\n";
      out.write(reinterpret_cast<const char *>(buffer), width*height*3);
      }
      else
      if(bpp == 8)
      {
      out << "P5\n" << width << ' ' << height << "\n255\n";
      out.write(reinterpret_cast<const char *>(buffer), width*height);	
      }
      else
      {
	std::cerr << "Invalid bpp = " << bpp << std::endl;
	return false;
      }
      return true;
      }
    else
    return false;

  }

static void pnm_skip_comments(std::istream & i)
{
 while (isspace(i.peek()))
    {
    while (isspace(i.peek()))
        i.get();
    if (i.peek() == '#')
      while (i.peek()!='\r' && i.peek()!='\n')
           { i.get(); }
       }
}

static char pnm_read_header(unsigned int & width, unsigned int &height, std::istream &iss)
{
  char h, t; 
  int max_val;

  // check pnm header
  h = iss.get();
  t = iss.get();
  if(!((h=='P') && ((t=='5')||(t=='6'))))
    return '\0';

  pnm_skip_comments(iss);
  iss>>width;
  pnm_skip_comments(iss);
  iss>>height;
  pnm_skip_comments(iss);
  iss>>max_val;
  iss.get(); // TODO: use a getline fn 
  return t;
}

bool Image::Load(const char *file)
{
  std::ifstream istr(file);
  unsigned int image_width, image_height;
  char header = pnm_read_header(image_width,image_height,istr);
  
  if(header == '6')  {
    Build(image_width, image_height, 24);
    istr.read(reinterpret_cast<char *>(buffer),  width * height*3);
    }
  else
  if(header == '5'){
    Build(image_width, image_height, 8);
    istr.read(reinterpret_cast<char *>(buffer),  width * height);    
  }
  else
  {
    std::cerr << file << " is not a valid PGM/PPM file" << std::endl;
    return false;
  }
  return true;
}

#endif
