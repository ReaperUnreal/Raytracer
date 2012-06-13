#ifndef RENDERSURFACE_H
#define RENDERSURFACE_H

#include "main.h"

class RenderSurface
{
public:
	//constructor and destructor
	RenderSurface(int w, int h);
	virtual ~RenderSurface(void);

	//accessor methods
	Color* GetSurface(void);
	int GetWidth(void) const;
	int GetHeight(void) const;

	//graphics methods
	void PlotPixel(const Color &c, const int x, const int y);
	void ToIntSurface(int *iSurface) const;
	void ToSDLSurface(SDL_Surface *sSurface) const;
private:
	Color *surface;
	int width, height;
};

#endif