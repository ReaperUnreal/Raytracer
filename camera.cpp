#include "main.h"

Camera::Camera(float fieldofview, Vector c, Vector u, Vector n) : position(c), up(u), normal(n)
{
	//store the fov
	fov = fieldofview;

	//calculate the right vector
	right = normal.Cross(up);

	//normalize all of the vectors
	up /= up.Length();
	normal /= normal.Length();
	right /= right.Length();
}

Camera::~Camera(void)
{
}

void Camera::Move(Vector d)
{
	position += d;
}

void Camera::MoveTo(Vector p)
{
	position = p;
}

void Camera::LookAt(Vector look, Vector upwards)
{
	//calculate the vectors
	normal = look - position;
	up = upwards;
	right = normal.Cross(up);

	//normalize all of the vectors
	up /= up.Length();
	normal /= normal.Length();
	right /= right.Length();
}

void Camera::LookAtMove(Vector look, Vector pos, Vector upwards)
{
	//set the new position
	position = pos;

	//calculate the vectors
	normal = position - look;
	up = upwards;
	right = normal.Cross(up);

	//normalize all of the vectors
	up /= up.Length();
	normal /= normal.Length();
	right /= right.Length();
}

void Camera::SetFieldOfView(float angle)
{
	fov = angle;
}