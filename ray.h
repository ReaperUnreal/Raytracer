#ifndef RAY_H
#define RAY_H

#include "main.h"

class Ray
{
public:
	Ray(void);
	Ray(Vector start, Vector dir);
	virtual ~Ray(void);

	Vector origin;
	Vector direction;
};

#endif