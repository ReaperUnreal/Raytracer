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
