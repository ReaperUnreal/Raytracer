#include "main.h"

//constructor and destructor
RenderSurface::RenderSurface(int w, int h) : width(w), height(h)
{
	surface = new Color[width * height]();
}

RenderSurface::~RenderSurface(void)
{
	delete[] surface;
}

//accessor methods
Color* RenderSurface::GetSurface(void)
{
	return surface;
}

int RenderSurface::GetHeight(void) const
{
	return height;
}

int RenderSurface::GetWidth(void) const
{
	return width;
}

//graphics methods
void RenderSurface::PlotPixel(const Color &c, const int x, const int y)
{
	surface[x + y * width] = c;
}

void RenderSurface::ToIntSurface(int *iSurface) const
{
	int num = width * height;
	Color *iter = surface;
	for(int i = 0; i < num; i++, iter++, iSurface++)
	{
		(*iSurface) = (iter->ClampNormal()).ToInt();
	}
}

void RenderSurface::ToBMP(BMP &out) const
{
   //use EasyBMP to get all this output
   out.SetSize(width, height);
   out.SetBitDepth(24);

   Color *iter = surface;
   for(int j = 0; j < height; j++)
   {
      for(int i = 0; i < width; i++, iter++)
      {
         out.SetPixel(i, j, (iter->ClampNormal()).ToRGBAPixel());
      }
   }
}

void RenderSurface::ToRawFile(const char *filename) const
{
   //open the file
   std::ofstream file(filename, ofstream::binary);

   //check for failure
   if(file.fail())
   {
      fprintf(stderr, "Could not open file %s\n", filename);
      return;
   }

   //write the size
   file.write(reinterpret_cast<const char *>(&width), 4);
   file.write(reinterpret_cast<const char *>(&height), 4);

   //write the pixels
   int num = width * height;
   Color *iter = surface;
   for(int i = 0; i < num; i++, iter++)
   {
      //r, g, b, in that order
      float r = iter->rv();
      float g = iter->gv();
      float b = iter->bv();
	  file.write(reinterpret_cast<const char *>(&r), 4);
	  file.write(reinterpret_cast<const char *>(&g), 4);
	  file.write(reinterpret_cast<const char *>(&b), 4);
   }

   //done writing
   file.close();
}
