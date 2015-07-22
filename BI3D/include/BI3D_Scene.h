#ifndef BI3D_SCENE_INCLUDE_H
#define BI3D_SCENE_INCLUDE_H

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "BI3D_Camera.h"
#include "BI3D_Object.h"

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void AddChild(Object* object);
	void AddCamera(Camera* camera);

	void Update();

	void SetScreen(GLfloat width, GLfloat height);
private:
	Camera* mainCamera;
	vector<Object*> objectList;

	int screenWidth, screenHeight;
};

#endif