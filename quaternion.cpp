#include "main.h"
Quaternion::Quaternion(float _a = 0.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
{
   a = _a;
   x = _x;
   y = _y;
   z = _z;
}

Quaternion::Quaternion(float *arr)
{
   //I don't trust the compiler to optimize the explicit assignment properly
   memcpy(array, arr, 4 * sizeof(float));
}

virtual Quaternion::~Quaternion(void)
{
}

//accessors
float Quaternion::av() const
{
   return a;
}

float Quaternion::xv() const
{
   return x;
}

float Quaternion::yv() const
{
   return y;
}

float Quaternion::zv() const
{
   return z;
}

Vector Quaternion::v() const
{
   return Vector(x, y, z);
}


//operators
Quaternion Quaternion::operator+(const Quaternion &iq) const
{
   return Quaternion(a + iq.a, x + iq.x, y + iq.y, z + iq.z);
}

Quaternion Quaternion::operator-(const Quaternion &iq) const
{
   return Quaternion(a - iq.a, x - iq.x, y - iq.y, z - iq.z);
}

Quaternion Quaternion::operator*(const Quaternion &iq) const
{
}

Quaternion Quaternion::operator/(const Quaternion &iq) const
{
}

Quaternion Quaternion::operator=(const Quaternion &iq)
{
   //same reasoning as array constructor
   memcpy(array, iq.array, 4 * sizeof(float));
}

Quaternion Quaternion::operator*(float f) const
{
}

Quaternion Quaternion::operator/(float f) const
{
}

void Quaternion::operator+=(const Quaternion &iq)
{
   a += iq.a;
   x += iq.x;
   y += iq.y;
   z += iq.z;
}

void Quaternion::operator-=(const Quaternion &iq)
{
   a -= iq.a;
   x -= iq.x;
   y -= iq.y;
   z -= iq.z;
}

void Quaternion::operator*=(const Quaternion &iq)
{
}

void Quaternion::operator/=(const Quaternion &iq)
{
}

void Quaternion::operator*=(float f)
{
}

void Quaternion::operator/=(float f)
{
}

