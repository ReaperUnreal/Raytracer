#include "main.h"

//constructor and destructor
Scene::Scene(int initial_size)
{
	if(initial_size < 1)
		initial_size = 1;
	capacity = initial_size;
	numObjects = 0;
	objects = new Geometry*[capacity];
	Geometry **temp = objects;
	for(int i = 0; i < capacity; i++, temp++)
	{
		*temp = NULL;
	}
}

Scene::~Scene(void)
{
	if(objects)
	{
		delete[] objects;
		objects = NULL;
	}
}

//useful methods
void Scene::Clear(void)
{
	Geometry **temp = objects;
	for(int i = 0; i < capacity; i++, temp++)
	{
		*temp = NULL;
	}
}

void Scene::DeleteAll(void)
{
	Geometry **temp = objects;
	for(int i = 0; i < numObjects; i++, temp++)
	{
		delete *temp;
		*temp = NULL;
	}
}

int Scene::AddObject(Geometry *object)
{
	//check for null
	if(!object)
	{
		return -1;
	}

	//adjust the capacity if needed
	if(numObjects == capacity)
	{
		Geometry **temp = new Geometry*[capacity];
		Geometry **iter1, **iter2;
		int i;
		for(i = 0, iter1 = temp, iter2 = objects; i < capacity; i++, iter1++, iter2++)
		{
			*iter1 = *iter2;
		}
		capacity *= 2;
		delete[] objects;
		objects = new Geometry*[capacity];
		for(i = 0, iter1 = objects, iter2 = temp; i < numObjects; i++, iter1++, iter2++)
		{
			*iter1 = *iter2;
		}
		delete[] temp;
	}

	//add the object
	objects[numObjects] = object;
	numObjects++;

	//return the number of the object
	return numObjects - 1;
}

void Scene::RemoveObject(Geometry *object)
{
	//check for null
	if(!object)
	{
		return;
	}

	//find the object and remove it
	Geometry **iter1 = objects, **iter2 = NULL;
   int i, j;
	for(i = 0; i < numObjects; i++, iter1++)
	{
		//found the object
		if(object == *iter1)
		{
			//remove it
			*iter1 = NULL;
			iter2 = iter1;
			iter1++;
			for(j = 0; j < numObjects; j++, iter1++)
			{
				*iter2 = *iter1;
				iter2 = iter1;
			}
			*iter2 = NULL;
			numObjects--;

			//resize if necessary
			if(numObjects < capacity / 2)
			{
				Geometry **temp = new Geometry*[numObjects];
				for(j = 0, iter1 = temp, iter2 = objects; j < numObjects; j++, iter1++, iter2++)
				{
					*iter1 = *iter2;
				}
				capacity /= 2;
				delete[] objects;
				objects = new Geometry*[capacity];
				for(j = 0, iter1 = objects, iter2 = temp; j < numObjects; j++, iter1++, iter2++)
				{
					*iter1 = *iter2;
				}
				delete[] temp;
			}

			break;
		}
	}
}

Geometry* Scene::RemoveObjectAt(int index)
{
	//check for valid range
	if((index < 0) || (index > numObjects))
	{
		return NULL;
	}

	//find the object and remove it
	Geometry **iter1 = (objects + index), **iter2 = NULL, *retval = NULL;
	retval = *iter1;
	*iter1 = NULL;

	//adjust the list
	iter2 = iter1;
	iter1++;
   int i;
	for(i = 0; i < numObjects; i++, iter1++)
	{
		*iter2 = *iter1;
		iter2 = iter1;
	}
	*iter2 = NULL;
	numObjects--;

	//resize if necessary
	if(numObjects < capacity / 2)
	{
		Geometry **temp = new Geometry*[numObjects];
		for(i = 0, iter1 = temp, iter2 = objects; i < numObjects; i++, iter1++, iter2++)
		{
			*iter1 = *iter2;
		}
		capacity /= 2;
		delete[] objects;
		objects = new Geometry*[capacity];
		for(i = 0, iter1 = objects, iter2 = temp; i < numObjects; i++, iter1++, iter2++)
		{
			*iter1 = *iter2;
		}
		delete[] temp;
	}

	return retval;
}

//get methods
Geometry* Scene::GetObjectAt(int index)
{
	return *(objects + index);
}

Geometry** Scene::GetObjects(void)
{
	return objects;
}

int Scene::GetNumObjects(void) const
{
	return numObjects;
}
