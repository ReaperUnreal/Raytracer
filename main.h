#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <string>
#include <stdio.h>
#ifdef OMP_ENABLE
   #include <omp.h>
#endif
#ifdef SSE2_ENABLE
   #include <xmmintrin.h>
   #include <intrin.h>
#endif
#ifdef __linux__
   #include <sys/time.h>
   #include <unistd.h>
#endif
#include "EasyBMP.h"

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
#ifndef __linux__
	if(condition)
	{
		__asm
		{
			int 3
		}
	}
#endif
}

#ifndef __linux__
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
#endif

inline float lrflti(int intgr)
{
#ifndef __linux__
	float flt;
	__asm
	{
		fild	intgr
		fstp	flt
	}
	return flt;
#else
   return static_cast<float>(intgr);
#endif
}

inline unsigned int CreateColor(int r, int g, int b)
{
	return ((r << 16) + (g << 8) + b);
}

#ifdef SSE2_ENABLE
	#include "color_sse.h"
	#include "vector_sse.h"
#else
	#include "color.h"
	#include "vector.h"
#endif
#include "rendersurface.h"
#include "ray.h"
#include "camera.h"
#include "geometry.h"
#include "scene.h"
#include "raytracer.h"

#endif
