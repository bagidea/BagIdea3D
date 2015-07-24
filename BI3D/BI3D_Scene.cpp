#include "BI3D_Scene.h"

Prefab::Prefab(Material* material, Object* object, string name)
{
	this->material = material;
	this->object = object;
	this->name = name;
}

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
		if(objectList[i] != NULL)
		{
			delete objectList[i];
			objectList[i] = NULL;
		}
	}

	objectList.clear();

	for(GLint i = 0; i < materialList.size(); i++)
	{
		if(materialList[i] != NULL)
		{
			delete materialList[i];
			materialList[i] = NULL;
		}
	}

	materialList.clear();

	for(GLint i = 0; i < prefabList.size(); i++)
	{
		if(prefabList[i] != NULL)
		{
			if(prefabList[i]->object != NULL)
			{
				delete prefabList[i]->object;
				prefabList[i]->object = NULL;
			}

			delete prefabList[i];
			prefabList[i] = NULL;
		}
	}

	prefabList.clear();

	if(mainCamera != NULL)
	{
		delete mainCamera;
		mainCamera = NULL;
	}
}

void Scene::AddChild(Object* object)
{
	if(!object->GetClone())
	{
		materialList.push_back(object->GetMaterial());
	}

	objectList.push_back(object);
}

void Scene::DeleteChild(Object* object)
{
	for(GLint i = 0; i < objectList.size(); i++)
	{
		if(objectList[i] == object)
		{
			if(objectList[i] != NULL)
			{
				delete objectList[i];
				objectList[i] = NULL;
			}

			objectList.erase(objectList.begin()+i);
		}
	}
}

void Scene::CreatePrefab(Object* object, string name)
{
	bool exist = false;

	for(GLint i = 0; i < materialList.size(); i++)
	{
		if(materialList[i] == object->GetMaterial())
			exist = true;
	}

	if(!exist)
		materialList.push_back(object->GetMaterial());

	prefabList.push_back(new Prefab(object->GetMaterial(), object, name));
}

void Scene::DeletePrefab(string name)
{
	for(GLint i = 0; i < prefabList.size(); i++)
	{
		if(prefabList[i]->name == name)
		{
			for(GLint a = 0; a < objectList.size(); a++)
			{
				if(objectList[a]->GetMesh() == prefabList[i]->object->GetMesh())
				{
					if(objectList[a] != NULL)
					{
						delete objectList[a];
						objectList[a] = NULL;
					}

					objectList.erase(objectList.begin()+a);
				}
			}
		}
	}
}

Object* Scene::GetPrefab(string name)
{
	for(GLint i = 0; i < prefabList.size(); i++)
	{
		if(prefabList[i]->name == name)
			return prefabList[i]->object;
	}

	return NULL;
}

void Scene::AddCamera(Camera* camera)
{
	GLfloat ratio = (GLfloat)screenWidth / (GLfloat)screenHeight;
	camera->SetRatio(ratio);
	mainCamera = camera;
}

void Scene::Update()
{
	glm::mat4 _projection;
	glm::mat4 _view;

	_projection = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);
	_view = mainCamera->GetTransform();

	for(GLint i = 0; i < materialList.size(); i++)
	{
		materialList[i]->Bind();

		glUniformMatrix4fv(glGetUniformLocation(materialList[i]->program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
		glUniformMatrix4fv(glGetUniformLocation(materialList[i]->program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

		for(GLint a = 0; a < objectList.size(); a++)
		{
			if(objectList[a]->GetMaterial() == materialList[i])
				objectList[a]->Update();
		}
	}
}

void Scene::SetScreen(GLfloat width, GLfloat height)
{
	screenWidth = width;
	screenHeight = height;
}