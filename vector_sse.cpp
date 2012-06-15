#include "main.h"

//constructor and destructor
Vector::Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(0.0f)
{
}

Vector::Vector(float *a)
{
	__m128 v = _mm_loadu_ps(a);
	_mm_storeu_ps(array, v);
}

Vector::Vector(__m128 v)
{
	_mm_storeu_ps(array, v);
}

Vector::~Vector(void)
{
}

//the operators
Vector Vector::operator +(const Vector &v) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_loadu_ps(v.array);
	return Vector(_mm_add_ps(v1, v2));
}

Vector Vector::operator -(const Vector &v) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_loadu_ps(v.array);
	return Vector(_mm_sub_ps(v1, v2));
}

Vector Vector::operator *(float f) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_load1_ps(&f);
	return Vector(_mm_mul_ps(v1, v2));
}

Vector Vector::operator =(const Vector &v)
{
	__m128 m = _mm_loadu_ps(v.array);
	_mm_storeu_ps(array, m);
	return *this;
}

Vector operator *(float f, const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(v.array);
	__m128 v2 = _mm_load1_ps(&f);
	return Vector(_mm_mul_ps(v1, v2));
}

Vector Vector::operator /(float f) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_load1_ps(&f);
	return Vector(_mm_div_ps(v1, v2));
}

Vector operator /(float f, const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(v.array);
	__m128 v2 = _mm_load1_ps(&f);
	return Vector(_mm_div_ps(v1, v2));
}

void Vector::operator *=(float f)
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_load1_ps(&f);
	__m128 res = _mm_mul_ps(v1, v2);
	_mm_storeu_ps(array, res);
}

void Vector::operator /=(float f)
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_load1_ps(&f);
	__m128 res = _mm_div_ps(v1, v2);
	_mm_storeu_ps(array, res);
}

void Vector::operator +=(const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_loadu_ps(v.array);
	__m128 res = _mm_add_ps(v1, v2);
	_mm_storeu_ps(array, res);
}

void Vector::operator -=(const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_loadu_ps(v.array);
	__m128 res = _mm_sub_ps(v1, v2);
	_mm_storeu_ps(array, res);
}

float Vector::Length(void) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 res = _mm_mul_ps(v1, v1);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	res = _mm_sqrt_ss(res);
	return res.m128_f32[0];
	//return sqrtf(x * x + y * y + z * z);
}

float Vector::SqrLength(void) const
{
	__m128 v = _mm_loadu_ps(array);
	__m128 res = _mm_mul_ps(v, v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	return res.m128_f32[0];
	//return x * x + y * y + z * z;
}

float Vector::Dot(const Vector &v) const
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 v2 = _mm_loadu_ps(v.array);
	__m128 res = _mm_mul_ps(v1, v2);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	return res.m128_f32[0];
	//return x * v.x + y * v.y + z * v.z;
}

Vector Vector::Cross(const Vector &v) const
{
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void Vector::Normalize(void)
{
	__m128 v1 = _mm_loadu_ps(array);
	__m128 res = _mm_mul_ps(v1, v1);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	res = _mm_rsqrt_ps(res);
	res = _mm_mul_ps(v1, res);
	_mm_storeu_ps(array, res);

	/*float len = sqrtf(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;*/
}

float Vector::Length(const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(v.array);
	__m128 res = _mm_mul_ps(v1, v1);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	res = _mm_sqrt_ss(res);
	return res.m128_f32[0];
	//return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vector::SqrLength(const Vector &v)
{
	__m128 v1 = _mm_loadu_ps(v.array);
	__m128 res = _mm_mul_ps(v1, v1);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	return res.m128_f32[0];
	//return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector::Dot(const Vector &v1, const Vector &v2)
{
	__m128 v1a = _mm_loadu_ps(v1.array);
	__m128 v2a = _mm_loadu_ps(v2.array);
	__m128 res = _mm_mul_ps(v1a, v2a);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
	return res.m128_f32[0];
	//return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector Vector::Cross(const Vector &v1, const Vector &v2)
{
	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

void Vector::Normalize(Vector &v)
{
	v.Normalize();
}
