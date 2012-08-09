#include "main.h"

//constructor and destructor
Vector::Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
{
}

Vector::~Vector(void)
{
}

//the accessors
float Vector::xv() const
{
   return x;
}

float Vector::yv() const
{
   return y;
}

float Vector::zv() const
{
   return z;
}

void Vector::Print() const
{
   printf("[%0.3f, %0.3f, %0.3f]\n", x, y, z);
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

Vector Vector::operator *(const Vector &v) const
{
   return Vector(x * v.x, y * v.y, z * v.z);
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

void Vector::operator *=(const Vector &v)
{
   x *= v.x;
   y *= v.y;
   z *= v.z;
}

float Vector::Length(void) const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector::LengthN(int n) const
{
   float powx = x;
   float powy = y;
   float powz = z;
   float invpow = 1.0f / static_cast<float>(n);
   for(int i = 1; i < n; i++)
   {
      powx *= x;
      powy *= y;
      powz *= z;
   }
   float sum = powx + powy + powz;
   return powf(sum, invpow);
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

float Vector::LengthN(const Vector &v, int n)
{
   float powx = v.x;
   float powy = v.y;
   float powz = v.z;
   float invpow = 1.0f / static_cast<float>(n);
   for(int i = 1; i < n; i++)
   {
      powx *= v.x;
      powy *= v.y;
      powz *= v.z;
   }
   float sum = powx + powy + powz;
   return powf(sum, invpow);
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

Vector Vector::Min(const Vector &v1, const Vector &v2)
{
   Vector m(v1);
   if(v2.x < m.x)
      m.x = v2.x;
   if(v2.y < m.y)
      m.y = v2.y;
   if(v2.z < m.z)
      m.z = v2.z;
   return m;
}

Vector Vector::Max(const Vector &v1, const Vector &v2)
{
   Vector m(v1);
   if(v2.x > m.x)
      m.x = v2.x;
   if(v2.y > m.y)
      m.y = v2.y;
   if(v2.z > m.z)
      m.z = v2.z;
   return m;
}

Vector Vector::Reciprocal() const
{
   return Vector(1.0f / x, 1.0f / y, 1.0f / z);
}

Vector Vector::Reciprocal(const Vector &v)
{
   return Vector(1.0f / v.x, 1.0f / v.y, 1.0f / v.z);
}

Vector Vector::Abs() const
{
   return Vector(fabsf(x), fabsf(y), fabsf(z));
}

Vector Vector::Abs(const Vector &v)
{
   return Vector(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}

float Vector::MaxComp() const
{
   return fmax(fmax(x, y), z);
}

float Vector::MaxComp(const Vector &v)
{
   return fmax(fmax(v.x, v.y), v.z);
}

Vector Vector::Mod(const Vector &v, const Vector &c)
{
   float x = fmod(v.x, c.x);
   float y = fmod(v.y, c.y);
   float z = fmod(v.z, c.z);
   return Vector(x, y, z);
}

Vector Vector::Floor() const
{
   float nx = floor(x);
   float ny = floor(y);
   float nz = floor(z);
   return Vector(nx, ny, nz);
}

Vector Vector::Floor(const Vector &v)
{
   float nx = floor(v.x);
   float ny = floor(v.y);
   float nz = floor(v.z);
   return Vector(nx, ny, nz);
}
