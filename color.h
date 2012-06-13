#ifndef SSE2_ENABLE
#ifndef COLOR_H
#define COLOR_H

#include "main.h"

class Color
{
public:
	//constructor and destructor
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f);
	~Color(void);

	//operators
	Color operator+(const Color &c) const;
	Color operator-(const Color &c) const;
	Color operator*(const Color &c) const;
	Color operator/(const Color &c) const;
	Color operator*(const float f) const;
	Color operator/(const float f) const;
	
	void operator+=(const Color &c);
	void operator-=(const Color &c);
	void operator*=(const Color &c);
	void operator/=(const Color &c);
	void operator*=(const float f);
	void operator/=(const float f);

	//other functions
	Color ClampUpper(float f);
	Color ClampLower(float f);
	Color ClampNormal(void);
	int ToInt(void);

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
	float r, g, b;
};

#endif
#endif