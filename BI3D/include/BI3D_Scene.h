#ifndef BI3D_SCENE_INCLUDE_H
#define BI3D_SCENE_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <GL/glew.h>

#include "BI3D_Camera.h"
#include "BI3D_Material.h"
#include "BI3D_Sprite2D.h"
#include "BI3D_FrameBuffer.h"
#include "BI3D_ShadowMap.h"
#include "BI3D_Object.h"

using namespace std;

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
	bool supportShadowMap;

	Scene();
	~Scene();

	void AddChild(Object* object);
	void AddChild(Sprite2D* sprite2D);
	void DeleteChild(Object* object);
	void DeleteChild(Sprite2D* sprite2D);

	void AddFrameBuffer(FrameBuffer* frameBuffer);
	void DeleteFrameBuffer(FrameBuffer* frameBuffer);

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

	void Update(Color bgColor);

	void SetScreen(GLfloat width, GLfloat height);
private:
	Camera* mainCamera;

	vector<Prefab*> prefabList;
	vector<Material*> materialList;
	vector<Object*> objectList;

	Material* sprite2DProgram;

	vector<Sprite2D*> sprite2DList;
	vector<FrameBuffer*> frameBufferList;

	//Light
	DirectionalLight* directionalLight;
	GLint maxPointLight;
	vector<PointLight*> pointLight;
	GLint maxSpotLight;
	vector<SpotLight*> spotLight;

	glm::mat4 _lightSpace;
	ShadowMap* shadowMap;

	//Screen Resolution
	int screenWidth, screenHeight;

	void Render(glm::mat4 _projection, glm::mat4 _view);
};

#endif