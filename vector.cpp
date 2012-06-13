#ifndef SSE2_ENABLE
#include "main.h"

//constructor and destructor
Vector::Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

Vector::~Vector(void)
{
}

//the operators
Vector Vector::operator +(const Vector &v) const
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator -(const Vector &v) const
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator *(float f) const
{
	return Vector(f * x, f * y, f * z);
}

Vector Vector::operator =(const Vector &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector operator *(float f, const Vector &v)
{
	return Vector(f * v.x, f * v.y, f * v.z);
}

Vector Vector::operator /(float f) const
{
	return Vector(x / f, y / f, z / f);
}

Vector operator /(float f, const Vector &v)
{
	return Vector(v.x / f, v.y / f, v.z / f);
}

void Vector::operator *=(float f)
{
	x *= f;
	y *= f;
	z *= f;
}

void Vector::operator /=(float f)
{
	x /= f;
	y /= f;
	z /= f;
}

void Vector::operator +=(const Vector &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector::operator -=(const Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

float Vector::Length(void) const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector::SqrLength(void) const
{
	return x * x + y * y + z * z;
}

float Vector::Dot(const Vector &v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::Cross(const Vector &v) const
{
	return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void Vector::Normalize(void)
{
	float len = sqrtf(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;

}

float Vector::Length(const Vector &v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vector::SqrLength(const Vector &v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector::Dot(const Vector &v1, const Vector &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector Vector::Cross(const Vector &v1, const Vector &v2)
{
	return Vector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

void Vector::Normalize(Vector &v)
{
	v.Normalize();
}
#endif