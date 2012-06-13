#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <string>
#include "SDL.h"
#include <omp.h>
#include <xmmintrin.h>
#include <intrin.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const float PI = 3.14159265358979323846264338327950288419716939937511f; //lol, 50 digits!
const float TWOPI = PI * 2.0f;
const float PIBYTWO = PI * 0.5f;
const float EPSILON = 0.00001f;
const float MAX_RAND_DIVIDER = 1.0f / RAND_MAX;

inline bool FloatEquals(float a, float b)
{
	float diff = a - b;
	return ((diff < EPSILON) && (diff > -EPSILON));
}

inline void BreakIf(bool condition)
{
	if(condition)
	{
		__asm
		{
			int 3
		}
	}
}

inline int lrintf(float flt)
{
	int intgr;
	__asm
	{
		fld		flt
		fistp	intgr
	}
	return intgr;
}

inline float lrflti(int intgr)
{
	float flt;
	__asm
	{
		fild	intgr
		fstp	flt
	}
	return flt;
}

inline unsigned int CreateColor(int r, int g, int b)
{
	return ((r << 16) + (g << 8) + b);
}

#ifdef SSE2_ENABLE
	#include "Color_SSE.h"
	#include "Vector_SSE.h"
#else
	#include "Color.h"
	#include "Vector.h"
#endif
#include "RenderSurface.h"
#include "Ray.h"
#include "Camera.h"
#include "Geometry.h"
#include "Scene.h"
#include "Raytracer.h"

#endif