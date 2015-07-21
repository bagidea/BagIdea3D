#ifndef BI3D_SCENE_INCLUDE_H
#define BI3D_SCENE_INCLUDE_H

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "BI3D_Object.h"

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void AddChild(Object* object);

	void Update();
private:
	vector<Object*> objectList;
};

#endif