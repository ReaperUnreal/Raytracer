#ifndef SSE2_ENABLE
#include "main.h"

//constructor and destructor
Color::Color(float red, float green, float blue) : r(red), g(green), b(blue)
{
}

Color::~Color(void)
{
}

//operators
Color Color::operator *(const Color &c) const
{
	return Color(r * c.r, g * c.g, b * c.b);
}

void Color::operator *=(const Color &c)
{
	r *= c.r;
	g *= c.g;
	b *= c.b;
}

Color Color::operator *(const float f) const
{
	return Color(f * r, f * g, f * b);
}

void Color::operator *=(const float f)
{
	r *= f;
	g *= f;
	b *= f;
}

Color operator *(const float f, const Color &c)
{
	return Color(f * c.r, f * c.g, f * c.b);
}

Color Color::operator +(const Color &c) const
{
	return Color(r + c.r, g + c.g, b + c.g);
}

void Color::operator +=(const Color &c)
{
	r += c.r;
	g += c.g;
	b += c.b;
}

Color Color::operator -(const Color &c) const
{
	return Color(r - c.r, g - c.g, b - c.b);
}

void Color::operator -=(const Color &c)
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
}

Color Color::operator /(const Color &c) const
{
	return Color(r / c.r, g / c.g, b / c.b);
}

Color Color::operator /(const float f) const
{
	return Color(r / f, g / f, b / f);
}

Color operator /(const float f, const Color &c)
{
	return Color(c.r / f, c.g / f, c.b / f);
}

void Color::operator /=(const Color &c)
{
	r /= c.r;
	g /= c.g;
	b /= c.g;
}

void Color::operator /=(const float f)
{
	r /= f;
	g /= f;
	b /= f;
}

Color Color::ClampLower(float f)
{
	Color temp(r, g, b);
	if(temp.r < f)
		temp.r = f;
	if(temp.g < f)
		temp.g = f;
	if(temp.b < f)
		temp.b = f;

	return temp;
}
Color Color::ClampUpper(float f)
{
	Color temp(r, g, b);
	if(temp.r > f)
		temp.r = f;
	if(temp.g > f)
		temp.g = f;
	if(temp.b > f)
		temp.b = f;

	return temp;
}
Color Color::ClampNormal(void)
{
	Color temp(r, g, b);
	if(temp.r > 1.0f)
		temp.r = 1.0f;
	if(temp.r < 0.0f)
		temp.r = 0.0f;
	if(temp.g > 1.0f)
		temp.g = 1.0f;
	if(temp.g < 0.0f)
		temp.g = 0.0f;
	if(temp.b > 1.0f)
		temp.b = 1.0f;
	if(temp.b < 0.0f)
		temp.b = 0.0f;

	return temp;
}

int Color::ToInt(void)
{
	int red = lrintf(r * 255.0f);
	int green = lrintf(g * 255.0f);
	int blue = lrintf(b * 255.0f);
	return ((red << 16) + (green << 8) + blue);
}

//static colors
const Color Color::white =     Color(1.0f, 1.0f, 1.0f);
const Color Color::black =     Color(0.0f, 0.0f, 0.0f);
const Color Color::red   =     Color(1.0f, 0.0f, 0.0f);
const Color Color::green =     Color(0.0f, 1.0f, 0.0f);
const Color Color::blue  =     Color(0.0f, 0.0f, 1.0f);
const Color Color::yellow =    Color(1.0f, 1.0f, 0.0f);
const Color Color::turquoise = Color(0.0f, 1.0f, 1.0f);
const Color Color::purple =    Color(1.0f, 0.0f, 1.0f);
#endif