#include "main.h"

//constructor and destructor
Scene::Scene(int initial_size)
{
	if(initial_size < 1)
		initial_size = 1;
	numObjects = 0;
   objects.reserve(initial_size);
}

Scene::~Scene(void)
{
}

//useful methods
void Scene::DeleteAll()
{
   while(!objects.empty())
   {
      Geometry *g = objects.back();
      objects.pop_back();
      delete g;
   }
}

int Scene::AddObject(Geometry *object)
{
	//add the object
   objects.push_back(object);
	numObjects++;

	//return the number of the object
	return numObjects - 1;
}

//get methods
Geometry* Scene::GetObjectAt(int index)
{
	return objects[index];
}

vector<Geometry *>& Scene::GetObjects(void)
{
	return objects;
}

int Scene::GetNumObjects(void) const
{
	return numObjects;
}

//preprocessing
void Scene::Preprocess()
{
   for(int i = 0; i < numObjects; ++i)
   {
      objects[i]->Preprocess();
   }
}
