#ifndef SCENE_H
#define SCENE_H

#include "main.h"

class Scene
{
public:
	Scene(int initial_size = 16);
	~Scene(void);

	int AddObject(Geometry *object);
	void RemoveObject(Geometry *object);
	Geometry* RemoveObjectAt(int index);
	void Clear(void);
	void DeleteAll(void);

	Geometry* GetObjectAt(int index);
	Geometry** GetObjects(void);
	int GetNumObjects(void) const;
private:
	int numObjects;
	int capacity;
	Geometry **objects;
};

#endif