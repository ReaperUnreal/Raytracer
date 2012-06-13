#ifndef CAMERA_H
#define CAMERA_H

#include "main.h"

class Camera
{
public:
	//constructor and destructor
	Camera(float fieldofview = PI * 0.25f, Vector c = Vector(0.0f, 0.0f, 0.0f), Vector u = Vector(0.0f, 1.0f, 0.0f), Vector n = Vector(0.0f, 0.0f, -1.0f));
	virtual ~Camera(void);

	//useful methods
	void MoveTo(Vector p);
	void Move(Vector d);
	void LookAtMove(Vector look, Vector pos, Vector upwards = Vector(0.0f, 1.0f, 0.0f));
	void LookAt(Vector look, Vector upwards = Vector(0.0f, 1.0f, 0.0f));
	void SetFieldOfView(float angle = PI * 0.25);

	//memory
	Vector position;
	Vector up, normal, right;
	float fov;
};

#endif