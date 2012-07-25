#ifndef QUATERNION_H
#define QUATERNION_H

#include "main.h"

class Quaternion
{
public:
	//constructor and destructor
	Quaternion(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
	Quaternion(float *a);
#ifdef SSE2_ENABLE
	Quaternion(__m128 iv);
#endif

	virtual ~Quaternion(void);

   //accessors
   float av() const;
   float xv() const;
   float yv() const;
   float zv() const;
   Vector v() const;

	//operators
	Quaternion operator+(const Quaternion &iv) const;
	Quaternion operator-(const Quaternion &iv) const;
   Quaternion operator*(const Quaternion &iv) const;
	Quaternion operator=(const Quaternion &iv);
	Quaternion operator*(float f) const;
	Quaternion operator/(float f) const;

	void operator+=(const Quaternion &iv);
	void operator-=(const Quaternion &iv);
   void operator*=(const Quaternion &iv);
	void operator*=(float f);
	void operator/=(float f);

	//memory
#ifdef SSE2_ENABLE
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
