#include "BI3D_Scene.h"

Scene::Scene()
{
	mainCamera = NULL;
	screenWidth = 0.0f;
	screenHeight = 0.0f;
}

Scene::~Scene()
{
	for(GLint i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
		objectList[i] = NULL;
	}

	objectList.clear();

	delete mainCamera;
	mainCamera = NULL;
}

void Scene::AddChild(Object* object)
{
	objectList.push_back(object);
}

void Scene::AddCamera(Camera* camera)
{
	GLfloat ratio = (GLfloat)screenWidth / (GLfloat)screenHeight;
	camera->SetRatio(ratio);
	mainCamera = camera;
}

void Scene::Update()
{
	for(GLint i = 0; i < objectList.size(); i++)
		objectList[i]->Update(mainCamera);
}

void Scene::SetScreen(GLfloat width, GLfloat height)
{
	screenWidth = width;
	screenHeight = height;
}