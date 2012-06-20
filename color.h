#ifndef COLOR_H
#define COLOR_H

#include "main.h"

class Color
{
public:
	//constructor and destructor
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f);
#ifdef SSE2_ENABLE
	Color(__m128 ic);
	Color(float *a);
#endif

	~Color(void);

   //accessors
   float rv() const;
   float gv() const;
   float bv() const;
   float av() const;

	//operators
	Color operator+(const Color &ic) const;
	Color operator-(const Color &ic) const;
	Color operator*(const Color &ic) const;
	Color operator/(const Color &ic) const;
	Color operator*(const float f) const;
	Color operator/(const float f) const;
	
	void operator+=(const Color &ic);
	void operator-=(const Color &ic);
	void operator*=(const Color &ic);
	void operator/=(const Color &ic);
	void operator*=(const float f);
	void operator/=(const float f);

	//other functions
	Color ClampUpper(float f);
	Color ClampLower(float f);
	Color ClampNormal(void);
	int ToInt(void);
   RGBApixel ToRGBAPixel(void);

	//static memory
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color turquoise;
	static const Color purple;

	//memory
#ifdef SSE2_ENABLE
	union
	{
      __m128 c;
		float array[4];
	};
#else
   float r, g, b;
#endif
};

#endif
