#include "main.h"

//constructor and destructor
Vector::Vector(float _x, float _y, float _z)
{
   v = _mm_set_ps(_x, _y, _z, 0.0f);
}

Vector::Vector(float *a)
{
	v = _mm_loadu_ps(a);
}

Vector::Vector(__m128 iv)
{
	v = iv;
}

Vector::~Vector(void)
{
}

//the accessors
float Vector::x() const
{
   return array[3];
}

float Vector::y() const
{
   return array[2];
}

float Vector::z() const
{
   return array[1];
}

//the operators
Vector Vector::operator +(const Vector &iv) const
{
	return Vector(_mm_add_ps(v, iv.v));
}

Vector Vector::operator -(const Vector &iv) const
{
	return Vector(_mm_sub_ps(v, iv.v));
}

Vector Vector::operator *(float f) const
{
	__m128 vf = _mm_set1_ps(f);
	return Vector(_mm_mul_ps(v, vf));
}

Vector Vector::operator =(const Vector &iv)
{
   v = iv.v;
	return *this;
}

Vector operator *(float f, const Vector &iv)
{
	__m128 vf = _mm_set1_ps(f);
	return Vector(_mm_mul_ps(iv.v, vf));
}

Vector Vector::operator /(float f) const
{
	__m128 vf = _mm_set1_ps(f);
	return Vector(_mm_div_ps(v, vf));
}

Vector operator /(float f, const Vector &iv)
{
	__m128 vf = _mm_set1_ps(f);
	return Vector(_mm_div_ps(iv.v, vf));
}

void Vector::operator *=(float f)
{
	__m128 vf = _mm_set1_ps(f);
	v = _mm_mul_ps(v, vf);
}

void Vector::operator /=(float f)
{
	__m128 vf = _mm_load1_ps(&f);
	v = _mm_div_ps(v, vf);
}

void Vector::operator +=(const Vector &iv)
{
	v = _mm_add_ps(v, iv.v);
}

void Vector::operator -=(const Vector &iv)
{
	v = _mm_sub_ps(v, iv.v);
}

float Vector::Length(void) const
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(v, v, 0xEF);
#else
	__m128 res = _mm_mul_ps(v, v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	res = _mm_sqrt_ss(res);
	return reinterpret_cast<vector_access&>(res).array[0];
	//return sqrtf(x * x + y * y + z * z);
}

float Vector::SqrLength(void) const
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(v, v, 0xEF);
#else
	__m128 res = _mm_mul_ps(v, v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	return reinterpret_cast<vector_access&>(res).array[0];
	//return x * x + y * y + z * z;
}

float Vector::Dot(const Vector &iv) const
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(v, iv.v, 0xEF);
#else
	__m128 res = _mm_mul_ps(v, iv.v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	return reinterpret_cast<vector_access&>(res).array[0];
	//return x * v.x + y * v.y + z * v.z;
}

Vector Vector::Cross(const Vector &iv) const
{
   __m128 as1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 3, 0));
   __m128 as2 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 3, 2, 0));
   __m128 bs1 = _mm_shuffle_ps(iv.v, iv.v, _MM_SHUFFLE(1, 3, 2, 0));
   __m128 bs2 = _mm_shuffle_ps(iv.v, iv.v, _MM_SHUFFLE(2, 1, 3, 0));

   __m128 res = _mm_sub_ps(_mm_mul_ps(as1, bs1), _mm_mul_ps(as2, bs2));
   return Vector(res);
	//return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void Vector::Normalize(void)
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(v, v, 0xEF);
#else
	__m128 res = _mm_mul_ps(v, v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	res = _mm_rsqrt_ps(res);
	v = _mm_mul_ps(v, res);

	/*float len = sqrtf(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;*/
}

float Vector::Length(const Vector &iv)
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(iv.v, iv.v, 0xEF);
#else
	__m128 res = _mm_mul_ps(iv.v, iv.v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	res = _mm_sqrt_ss(res);
	return reinterpret_cast<vector_access&>(res).array[0];
	//return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vector::SqrLength(const Vector &iv)
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(iv.v, iv.v, 0xEF);
#else
	__m128 res = _mm_mul_ps(iv.v, iv.v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	return reinterpret_cast<vector_access&>(res).array[0];
	//return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector::Dot(const Vector &v1, const Vector &v2)
{
#ifdef __SSE4_1__
   __m128 res = _mm_dp_ps(v1.v, v2.v, 0xEF);
#else
	__m128 res = _mm_mul_ps(v1.v, v2.v);
	res = _mm_hadd_ps(res, res);
	res = _mm_hadd_ps(res, res);
#endif
	return reinterpret_cast<vector_access&>(res).array[0];
	//return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector Vector::Cross(const Vector &v1, const Vector &v2)
{
   __m128 as1 = _mm_shuffle_ps(v1.v, v1.v, _MM_SHUFFLE(2, 1, 3, 0));
   __m128 as2 = _mm_shuffle_ps(v1.v, v1.v, _MM_SHUFFLE(1, 3, 2, 0));
   __m128 bs1 = _mm_shuffle_ps(v2.v, v2.v, _MM_SHUFFLE(1, 3, 2, 0));
   __m128 bs2 = _mm_shuffle_ps(v2.v, v2.v, _MM_SHUFFLE(2, 1, 3, 0));

   __m128 res = _mm_sub_ps(_mm_mul_ps(as1, bs1), _mm_mul_ps(as2, bs2));
   return Vector(res);
	//return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

void Vector::Normalize(Vector &iv)
{
	iv.Normalize();
}

Vector Vector::Min(const Vector &v1, const Vector &v2)
{
   __m128 res = _mm_min_ps(v1.v, v2.v);
   return Vector(res);
}

Vector Vector::Max(const Vector &v1, const Vector &v2)
{
   __m128 res = _mm_max_ps(v1.v, v2.v);
   return Vector(res);
}
