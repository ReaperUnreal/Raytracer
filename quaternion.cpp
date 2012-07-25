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
   float na = a * iq.a - x * iq.x - y * iq.y - z * iq.z;
   float nx = a * iq.x + x * iq.a + y * iq.z - z * iq.y;
   float ny = a * iq.y - x * iq.z + y * iq.a + z * iq.x;
   float nz = a * iq.z + x * iq.y - y * iq.x + z * iq.a;

   return Quaternion(na, nx, ny, nz);
}

Quaternion Quaternion::operator/(const Quaternion &iq) const
{
   //there's way too much math to get wrong here, best not copy and paste
   return operator*(iq.Reciprocal());
}

Quaternion Quaternion::operator=(const Quaternion &iq)
{
   //same reasoning as array constructor
   memcpy(array, iq.array, 4 * sizeof(float));
}

Quaternion Quaternion::operator*(float f) const
{
   return Quaternion(a * f, x * f, y * f, z * f);
}

Quaternion Quaternion::operator/(float f) const
{
   float invf = 1.0f / f;
   return Quaternion(a * invf, x * invf, y * invf, z * invf);
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
   float na = a * iq.a - x * iq.x - y * iq.y - z * iq.z;
   float nx = a * iq.x + x * iq.a + y * iq.z - z * iq.y;
   float ny = a * iq.y - x * iq.z + y * iq.a + z * iq.x;
   float nz = a * iq.z + x * iq.y - y * iq.x + z * iq.a;

   a = na;
   x = nx;
   y = ny;
   z = nz;
}

void Quaternion::operator/=(const Quaternion &iq)
{
   //there's way too much math to get wrong here, best not copy and paste
   operator*=(iq.Reciprocal());
}

void Quaternion::operator*=(float f)
{
   a *= f;
   x *= f;
   y *= f;
   z *= f;
}

void Quaternion::operator/=(float f)
{
   float invf = 1.0f / f;
   a *= invf;
   x *= invf;
   y *= invf;
   z *= invf;
}

Quaternion Quaternion::Conjugate() const
{
   return Quaternion(a, -x, -y, -z);
}

Quaternion Quaternion::Reciprocal() const
{
   //defined as q^-1 = q* / ||q||^2
   // or the conjugate divided by the magnitude squared
   float mag2 = (a * a) + (x * x) + (y * y) + (z * z);
   float invmag2 = 1.0f / mag2;

   float na = a * invmag2;
   float nx = x * (-invmag2);
   float ny = y * (-invmag2);
   float nz = z * (-invmag2);

   return Quaternion(na, nx, ny, nz);
}

Quaternion Quaternion::Square() const
{
   float na = a * a - x * x - y * y - z * z;
   float nx = 2.0f * a * x;
   float ny = 2.0f * a * y;
   float nz = 2.0f * a * z;

   return Quaternion(na, nx, ny, nz);
}

float Quaternion::Magnitude() const
{
   return sqrtf((a * a) + (x * x) + (y * y) + (z * z));
}

float Quaternion::MagnitudeSquare() const
{
   return (a * a) + (x * x) + (y * y) + (z * z);
}

