#ifndef VECTOR_H
#define VECTOR_H

#include "main.h"

class Vector
{
public:
	//constructor and destructor
	Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
	Vector(float *a);
#ifdef SSE2_ENABLE
	Vector(__m128 iv);
#endif

	virtual ~Vector(void);

   //accessors
   float xv() const;
   float yv() const;
   float zv() const;

	//operators
	Vector operator+(const Vector &iv) const;
	Vector operator-(const Vector &iv) const;
   Vector operator*(const Vector &iv) const;
	Vector operator=(const Vector &iv);
	Vector operator*(float f) const;
	Vector operator/(float f) const;

	void operator+=(const Vector &iv);
	void operator-=(const Vector &iv);
   void operator*=(const Vector &iv);
	void operator*=(float f);
	void operator/=(float f);
	
	//other functions
	float Length(void) const;
	float SqrLength(void) const;
	float Dot(const Vector &iv) const;
	Vector Cross(const Vector &iv) const;
	void Normalize(void);
   Vector Reciprocal(void) const;
   Vector Abs(void) const;


	static float Length(const Vector &iv);
	static float SqrLength(const Vector &iv);
	static float Dot(const Vector &v1, const Vector &v2);
	static Vector Cross(const Vector &v1, const Vector &v2);
	static void Normalize(Vector &v);
   static Vector Min(const Vector &v1, const Vector &v2);
   static Vector Max(const Vector &v1, const Vector &v2);
   static Vector Reciprocal(const Vector &v);
   static Vector Abs(const Vector &v);

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
         float x, y, z, w;
      };
      float array[4];
   };
#endif
};

#endif
