#ifndef BI3D_SCENE_INCLUDE_H
#define BI3D_SCENE_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <GL/glew.h>

#include "BI3D_Camera.h"
#include "BI3D_Material.h"
#include "BI3D_Object.h"

using namespace std;

struct Color
{
	GLfloat r, g, b, a;
	Color();
	Color(GLfloat r, GLfloat g, GLfloat b);
	Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
};

struct Prefab
{
	Material* material;
	Object* object;
	string name;

	Prefab(Material* material, Object* object, string name);
};

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLfloat intensity;

	DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void SetDirection(GLfloat x, GLfloat y, GLfloat z);
	void SetAmbient(Color color);
	void SetColor(Color color);
	void SetSpecular(Color color);
};

struct PointLight
{
	GLfloat x, y ,z;

	GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLfloat intensity;

	PointLight(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	PointLight(GLfloat x, GLfloat y, GLfloat z);

	void SetAmbient(Color color);
	void SetColor(Color color);
	void SetSpecular(Color color);
};

struct SpotLight
{
	GLfloat x, y, z, rotationX, rotationY, rotationZ;
	glm::vec3 direction;

	GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

    GLfloat cutOff;
	GLfloat outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLfloat intensity;

	SpotLight(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, GLfloat cutOff, GLfloat outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	SpotLight(GLfloat x, GLfloat y, GLfloat z);

	void SetAmbient(Color color);
	void SetColor(Color color);
	void SetSpecular(Color color);
};

class Scene
{
public:
	Scene();
	~Scene();

	void AddChild(Object* object);
	void DeleteChild(Object* object);

	void AddMaterial(Material* material);
	void DeleteMaterial(Material* material);
	void CreatePrefab(Object* object, string name);
	void DeletePrefab(string name);
	Object* GetPrefab(string name);

	void SetDirectionalLightDirection(GLfloat x, GLfloat y, GLfloat z);
	void SetDirectionalLightAmbient(Color color);
	void SetDirectionalLightColor(Color color);
	void SetDirectionalLightSpecular(Color color);
	void SetDirectionalLightIntensity(GLfloat intensity);

	void AddPointLight(PointLight* light);
	void DeletePointLight(PointLight* light);

	void AddSpotLight(SpotLight* light);
	void DeleteSpotLight(SpotLight* light);

	void SetCamera(Camera* camera);
	void ClearCamera();

	void Update();

	void SetScreen(GLfloat width, GLfloat height);
	void SetGamma(GLfloat volume);
private:
	GLfloat gamma;

	Camera* mainCamera;

	vector<Prefab*> prefabList;
	vector<Material*> materialList;
	vector<Object*> objectList;

	//Light
	DirectionalLight* directionalLight;
	GLint maxPointLight;
	vector<PointLight*> pointLight;
	GLint maxSpotLight;
	vector<SpotLight*> spotLight;

	//Screen Resolution
	int screenWidth, screenHeight;
};

#endif