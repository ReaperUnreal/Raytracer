#ifndef QUATERNION_H
#define QUATERNION_H

#include "main.h"

class Quaternion
{
public:
	//constructor and destructor
	Quaternion(float _a = 0.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
	Quaternion(float *arr);
#ifdef SSE2_ENABLE_QUAT
	Quaternion(__m128 iq);
#endif

	virtual ~Quaternion(void);

   //accessors
   float av() const;
   float xv() const;
   float yv() const;
   float zv() const;
   Vector v() const;

	//operators
	Quaternion operator+(const Quaternion &iq) const;
	Quaternion operator-(const Quaternion &iq) const;
   Quaternion operator*(const Quaternion &iq) const;
   Quaternion operator/(const Quaternion &iq) const;
	Quaternion operator=(const Quaternion &iq);
	Quaternion operator*(float f) const;
	Quaternion operator/(float f) const;

	void operator+=(const Quaternion &iq);
	void operator-=(const Quaternion &iq);
   void operator*=(const Quaternion &iq);
   void operator/=(const Quaternion &iq);
	void operator*=(float f);
	void operator/=(float f);

   //various functions
   Quaternion Conjugate() const;
   Quaternion Reciprocal() const;
   Quaternion Square() const;
   float Magnitude() const;
   float MagnitudeSquare() const;

	//memory
#ifdef SSE2_ENABLE_QUAT
   union
   {
      __m128 v;
      float array[4];
   } __attribute__ ((aligned(16)));
#else
   union
   {
      struct
      {
         float a, x, y, z;
      };
      float array[4];
   };
#endif
};

#endif
