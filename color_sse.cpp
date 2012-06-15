#ifdef SSE2_ENABLE
#include "main.h"

//constructor and destructor
Color::Color(float red, float green, float blue)
{
   c = _mm_set_ps(red, green, blue, 0.0f);
}

Color::Color(float *ca)
{
   c = _mm_set_ps(ca[0], ca[1], ca[2], 0.0f);
}

Color::Color(__m128 ic)
{
   c = ic;
}

Color::~Color(void)
{
}


//accessors

float Color::r() const
{
   return array[3];
}

float Color::g() const
{
   return array[2];
}

float Color::b() const
{
   return array[1];
}

float Color::a() const
{
   return array[0];
}


//operators
Color Color::operator *(const Color &ic) const
{
	return Color(_mm_mul_ps(c, ic.c));
}

void Color::operator *=(const Color &ic)
{
	c = _mm_mul_ps(c, ic.c);
}

Color Color::operator *(const float f) const
{
	__m128 cf = _mm_set1_ps(f);
	return Color(_mm_mul_ps(c, cf));
}

void Color::operator *=(const float f)
{
	__m128 cf = _mm_set1_ps(f);
	c = _mm_mul_ps(c, cf);
}

Color operator *(const float f, const Color &ic)
{
	__m128 cf = _mm_set1_ps(f);
	return Color(_mm_mul_ps(ic.c, cf));
}

Color Color::operator +(const Color &ic) const
{
	return Color(_mm_add_ps(c, ic.c));
}

void Color::operator +=(const Color &ic)
{
	c = _mm_add_ps(c, ic.c);
}

Color Color::operator -(const Color &ic) const
{
	return Color(_mm_sub_ps(c, ic.c));
}

void Color::operator -=(const Color &ic)
{
	c = _mm_sub_ps(c, ic.c);
}

Color Color::operator /(const Color &ic) const
{
	return Color(_mm_div_ps(c, ic.c));
}

Color Color::operator /(const float f) const
{
	__m128 cf = _mm_set1_ps(f);
	return Color(_mm_sub_ps(c, cf));
}

Color operator /(const float f, const Color &ic)
{
	__m128 cf = _mm_set1_ps(f);
	return Color(_mm_sub_ps(ic.c, cf));
}

void Color::operator /=(const Color &ic)
{
	c = _mm_div_ps(c, ic.c);
}

void Color::operator /=(const float f)
{
	__m128 cf = _mm_set1_ps(f);
	c = _mm_div_ps(c, cf);
}

Color Color::ClampLower(float f)
{
	__m128 val = _mm_set1_ps(f);
	__m128 res = _mm_max_ps(c, val);

	return Color(res);
}
Color Color::ClampUpper(float f)
{
	__m128 val = _mm_set1_ps(f);
	__m128 res = _mm_min_ps(c, val);

	return Color(res);
}
Color Color::ClampNormal(void)
{
	__m128 zero = _mm_set1_ps(0.0f);
	__m128 one = _mm_set1_ps(1.0f);
	__m128 res = _mm_max_ps(c, zero);
	res = _mm_min_ps(res, one);

	return Color(res);
}

int Color::ToInt(void)
{
   __m128 mul = _mm_set1_ps(255.0f);
   mul = _mm_mul_ps(c, mul);
   float fr = reinterpret_cast<vector_access&>(mul).array[3];
   float fg = reinterpret_cast<vector_access&>(mul).array[2];
   float fb = reinterpret_cast<vector_access&>(mul).array[1];
	int red = lrintf(fr);
	int green = lrintf(fg);
	int blue = lrintf(fb);
	return ((red << 16) + (green << 8) + blue);
}

RGBApixel Color::ToRGBAPixel(void)
{
   __m128 mul = _mm_set1_ps(255.0f);
   mul = _mm_mul_ps(c, mul);
   float fr = reinterpret_cast<vector_access&>(mul).array[3];
   float fg = reinterpret_cast<vector_access&>(mul).array[2];
   float fb = reinterpret_cast<vector_access&>(mul).array[1];
   int red = lrintf(fr);
   int green = lrintf(fg);
   int blue = lrintf(fb);
   RGBApixel p;
   p.Red = red;
   p.Green = green;
   p.Blue = blue;
   p.Alpha = 255;
   return p;
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
