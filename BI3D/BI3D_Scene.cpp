#include "BI3D_Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for(GLint i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
		objectList[i] = NULL;
	}

	objectList.clear();
}

void Scene::AddChild(Object* object)
{
	objectList.push_back(object);
}

void Scene::Update()
{
	for(GLint i = 0; i < objectList.size(); i++)
		objectList[i]->Update();
}