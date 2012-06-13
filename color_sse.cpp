#ifdef SSE2_ENABLE
#include "main.h"

//constructor and destructor
Color::Color(float red, float green, float blue) : r(red), g(green), b(blue), a(0.0f)
{
}

Color::Color(float *ca)
{
	__m128 c = _mm_loadu_ps(ca);
	_mm_storeu_ps(array, c);
	a = 0.0f;
}

Color::Color(__m128 c)
{
	_mm_storeu_ps(array, c);
	a = 0.0f;
}

Color::~Color(void)
{
}

//operators
Color Color::operator *(const Color &c) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	return Color(_mm_mul_ps(c1, c2));
}

void Color::operator *=(const Color &c)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	__m128 res = _mm_mul_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

Color Color::operator *(const float f) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_load1_ps(&f);
	return Color(_mm_mul_ps(c1, c2));
}

void Color::operator *=(const float f)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_load1_ps(&f);
	__m128 res = _mm_mul_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

Color operator *(const float f, const Color &c)
{
	__m128 c1 = _mm_loadu_ps(c.array);
	__m128 c2 = _mm_load1_ps(&f);
	return Color(_mm_mul_ps(c1, c2));
}

Color Color::operator +(const Color &c) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	return Color(_mm_add_ps(c1, c2));
}

void Color::operator +=(const Color &c)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	__m128 res = _mm_add_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

Color Color::operator -(const Color &c) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	return Color(_mm_sub_ps(c1, c2));
}

void Color::operator -=(const Color &c)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	__m128 res = _mm_sub_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

Color Color::operator /(const Color &c) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_loadu_ps(c.array);
	return Color(_mm_div_ps(c1, c2));
}

Color Color::operator /(const float f) const
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_load1_ps(&f);
	return Color(_mm_sub_ps(c1, c2));
}

Color operator /(const float f, const Color &c)
{
	__m128 c1 = _mm_loadu_ps(c.array);
	__m128 c2 = _mm_load1_ps(&f);
	return Color(_mm_sub_ps(c1, c2));
}

void Color::operator /=(const Color &c)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_load1_ps(c.array);
	__m128 res = _mm_div_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

void Color::operator /=(const float f)
{
	__m128 c1 = _mm_loadu_ps(array);
	__m128 c2 = _mm_load1_ps(&f);
	__m128 res = _mm_div_ps(c1, c2);
	_mm_storeu_ps(array, res);
}

Color Color::ClampLower(float f)
{
	__m128 c = _mm_loadu_ps(array);
	__m128 val = _mm_load1_ps(&f);
	__m128 res = _mm_max_ps(c, val);

	return Color(res);
}
Color Color::ClampUpper(float f)
{
	__m128 c = _mm_loadu_ps(array);
	__m128 val = _mm_load1_ps(&f);
	__m128 res = _mm_min_ps(c, val);

	return Color(res);
}
Color Color::ClampNormal(void)
{
	__m128 c = _mm_loadu_ps(array);
	__m128 zero = _mm_set1_ps(0.0f);
	__m128 one = _mm_set1_ps(1.0f);
	__m128 res = _mm_max_ps(c, zero);
	res = _mm_min_ps(res, one);

	return Color(res);
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