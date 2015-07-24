#ifndef BI3D_SCENE_INCLUDE_H
#define BI3D_SCENE_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "BI3D_Camera.h"
#include "BI3D_Material.h"
#include "BI3D_Object.h"

using namespace std;

struct Prefab
{
	Material* material;
	Object* object;
	string name;

	Prefab(Material* material, Object* object, string name);
};

class Scene
{
public:
	Scene();
	~Scene();

	void AddChild(Object* object);
	void DeleteChild(Object* object);

	void CreatePrefab(Object* object, string name);
	void DeletePrefab(string name);
	Object* GetPrefab(string name);

	void AddCamera(Camera* camera);

	void Update();

	void SetScreen(GLfloat width, GLfloat height);
private:
	Camera* mainCamera;

	vector<Prefab*> prefabList;
	vector<Material*> materialList;
	vector<Object*> objectList;

	int screenWidth, screenHeight;
};

#endif