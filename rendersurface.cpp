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

void RenderSurface::ToSDLSurface(SDL_Surface *sSurface) const
{
	int sWidth = sSurface->pitch/4;
	unsigned int *pixels = (unsigned int *)sSurface->pixels;
	Color *iter = surface;
	int i, j;

	for(i = 0; i < height; i++, pixels += sWidth)
	{
		for(j = 0; j < width; j++, iter++)
		{
			*(pixels + j) = (iter->ClampNormal()).ToInt();
		}
	}
}