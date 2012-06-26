#ifndef SCENE_H
#define SCENE_H

#include "main.h"

class Scene
{
public:
	Scene(int initial_size = 16);
	~Scene(void);

	int AddObject(Geometry *object);
   void DeleteAll();

	Geometry* GetObjectAt(int index);
   vector<Geometry *>& GetObjects();
	int GetNumObjects() const;
private:
	int numObjects;
   vector<Geometry *> objects;
};

#endif
