#ifdef SSE2_ENABLE
#ifndef VECTOR_H
#define VECTOR_H

#include "main.h"

class Vector
{
public:
	//constructor and destructor
	Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
	Vector(float *a);
	Vector(__m128 v);
	virtual ~Vector(void);

	//operators
	Vector operator+(const Vector &v) const;
	Vector operator-(const Vector &v) const;
	Vector operator=(const Vector &v);
	Vector operator*(float f) const;
	Vector operator/(float f) const;

	void operator+=(const Vector &v);
	void operator-=(const Vector &v);
	void operator*=(float f);
	void operator/=(float f);
	
	//other functions
	float Length(void) const;
	float SqrLength(void) const;
	float Dot(const Vector &v) const;
	Vector Cross(const Vector &v) const;
	void Normalize(void);


	static float Length(const Vector &v);
	static float SqrLength(const Vector &v);
	static float Dot(const Vector &v1, const Vector &v2);
	static Vector Cross(const Vector &v1, const Vector &v2);
	static void Normalize(Vector &v);

	//memory
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float array[4];
	};
};

#endif
#endif