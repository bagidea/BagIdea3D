#include "BI3D_Scene.h"

Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 1.0f;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0f;
}

Color::Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Prefab::Prefab(Material* material, Object* object, string name)
{
	this->material = material;
	this->object = object;
	this->name = name;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->direction = direction;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->intensity = 1.0f;
}

void DirectionalLight::SetDirection(GLfloat x, GLfloat y, GLfloat z)
{
	direction = glm::vec3(x, y, z);
}

void DirectionalLight::SetAmbient(Color color)
{
	ambient = glm::vec3(color.r, color.g, color.b);
}

void DirectionalLight::SetColor(Color color)
{
	diffuse = glm::vec3(color.r, color.g, color.b);
}

void DirectionalLight::SetSpecular(Color color)
{
	specular = glm::vec3(color.r, color.g, color.b);
}

PointLight::PointLight(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->x = position.x;
	this->y = position.y;
	this->z = position.z;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->intensity = 1.0f;
}

PointLight::PointLight(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->constant = 1.0f;
	this->linear = 0.09f;
	this->quadratic = 0.032f;

	this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	this->intensity = 1.0f;
}

void PointLight::SetAmbient(Color color)
{
	ambient = glm::vec3(color.r, color.g, color.b);
}

void PointLight::SetColor(Color color)
{
	diffuse = glm::vec3(color.r, color.g, color.b);
}

void PointLight::SetSpecular(Color color)
{
	specular = glm::vec3(color.r, color.g, color.b);
}

SpotLight::SpotLight(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, GLfloat cutOff, GLfloat outerCutOff, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->x = position.x;
	this->y = position.y;
	this->z = position.z;

	this->direction = glm::vec3(this->x, this->y-1.0f, this->z);

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->intensity = 1.0f;
}

SpotLight::SpotLight(GLfloat x, GLfloat y, GLfloat z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->rotationX = 0.0f;
	this->rotationY = 0.0f;
	this->rotationZ = 0.0f;

	this->direction = glm::vec3(cosf(-rotationX*(MATH_PI/180.0f)) * sinf(-rotationY*(MATH_PI/180.0f)), sinf(-rotationX*(MATH_PI/180.0f)), cosf(-rotationX*(MATH_PI/180.0f)) * cosf(-rotationY*(MATH_PI/180.0f)));

	this->constant = 1.0f;
	this->linear = 0.09f;
	this->quadratic = 0.032f;

	this->cutOff = 12.5f;
	this->outerCutOff = 17.5f;

	this->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	this->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	this->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	this->intensity = 1.0f;
}

void SpotLight::SetAmbient(Color color)
{
	ambient = glm::vec3(color.r, color.g, color.b);
}

void SpotLight::SetColor(Color color)
{
	diffuse = glm::vec3(color.r, color.g, color.b);
}

void SpotLight::SetSpecular(Color color)
{
	specular = glm::vec3(color.r, color.g, color.b);
}

Scene::Scene()
{
	gamma = 2.0f;

	mainCamera = NULL;
	screenWidth = 0.0f;
	screenHeight = 0.0f;

	directionalLight = new DirectionalLight(glm::vec3(3.0f, -1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	maxPointLight = 4;
	maxSpotLight = 4;
}

Scene::~Scene()
{
	if(directionalLight != NULL)
	{
		delete directionalLight;
		directionalLight = NULL;
	}

	for(GLint i = 0; i < pointLight.size(); i++)
	{
		if(pointLight[i] != NULL)
		{
			delete pointLight[i];
			pointLight[i] = NULL;
		}
	}

	pointLight.clear();

	for(GLint i = 0; i < spotLight.size(); i++)
	{
		if(spotLight[i] != NULL)
		{
			delete spotLight[i];
			spotLight[i] = NULL;
		}
	}

	spotLight.clear();

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

void Scene::AddMaterial(Material* material)
{
	materialList.push_back(material);
}

void Scene::DeleteMaterial(Material* material)
{
	for(GLint i = 0; i < materialList.size(); i++)
	{
		if(materialList[i] == material)
		{
			delete materialList[i];
			materialList[i] = NULL;

			materialList.erase(materialList.begin()+i);
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

void Scene::SetDirectionalLightDirection(GLfloat x, GLfloat y, GLfloat z)
{
	directionalLight->SetDirection(x, y, z);
}

void Scene::SetDirectionalLightAmbient(Color color)
{
	directionalLight->ambient = glm::vec3(color.r, color.g, color.b);
}

void Scene::SetDirectionalLightColor(Color color)
{
	directionalLight->diffuse = glm::vec3(color.r, color.g, color.b);
}

void Scene::SetDirectionalLightSpecular(Color color)
{
	directionalLight->specular = glm::vec3(color.r, color.g, color.b);
}

void Scene::SetDirectionalLightIntensity(GLfloat intensity)
{
	directionalLight->intensity = intensity;
}

void Scene::AddPointLight(PointLight* light)
{
	pointLight.push_back(light);
}

void Scene::DeletePointLight(PointLight* light)
{
	for(GLint i = 0; i < pointLight.size(); i++)
	{
		if(pointLight[i] == light)
		{
			if(pointLight[i] != NULL)
				delete pointLight[i];

			pointLight[i] = NULL;
			pointLight.erase(pointLight.begin()+i);
		}
	}
}

void Scene::AddSpotLight(SpotLight* light)
{
	spotLight.push_back(light);
}

void Scene::DeleteSpotLight(SpotLight* light)
{
	for(GLint i = 0; i < spotLight.size(); i++)
	{
		if(spotLight[i] == light)
		{
			if(spotLight[i] != NULL)
				delete spotLight[i];

			spotLight[i] = NULL;
			spotLight.erase(spotLight.begin()+i);
		}
	}
}

void Scene::SetCamera(Camera* camera)
{
	GLfloat ratio = (GLfloat)screenWidth / (GLfloat)screenHeight;
	camera->SetRatio(ratio);
	mainCamera = camera;
}

void Scene::ClearCamera()
{
	if(mainCamera != NULL)
	{
		delete mainCamera;
		mainCamera = NULL;
	}
}

void Scene::Update()
{
	glm::mat4 _projection;
	glm::mat4 _view;

	_projection = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);
	
	if(mainCamera != NULL)
		_view = mainCamera->GetTransform();

	for(GLint i = 0; i < materialList.size(); i++)
	{
		materialList[i]->Bind();

		glUniform1f(glGetUniformLocation(materialList[i]->program, "gamma"), gamma);

		if(materialList[i]->GetType() == BI3D_SUPPORT_LIGHT || materialList[i]->GetType() == BI3D_SUPPORT_LIGHT_AND_NORMALMAP)
		{
			glUniform3f(glGetUniformLocation(materialList[i]->program, "directionalLight.direction"), directionalLight->direction.x, directionalLight->direction.y, directionalLight->direction.z);
			glUniform3f(glGetUniformLocation(materialList[i]->program, "directionalLight.ambient"), directionalLight->ambient.x, directionalLight->ambient.y, directionalLight->ambient.z);
			glUniform3f(glGetUniformLocation(materialList[i]->program, "directionalLight.diffuse"), directionalLight->diffuse.x*directionalLight->intensity, directionalLight->diffuse.y*directionalLight->intensity, directionalLight->diffuse.z*directionalLight->intensity);
			glUniform3f(glGetUniformLocation(materialList[i]->program, "directionalLight.specular"), directionalLight->specular.x, directionalLight->specular.y, directionalLight->specular.z);

			if(pointLight.size() < maxPointLight)
				glUniform1i(glGetUniformLocation(materialList[i]->program, "countPointLight"), pointLight.size());
			else
				glUniform1i(glGetUniformLocation(materialList[i]->program, "countPointLight"), maxPointLight);

			for(GLint a = 0; a < pointLight.size(); a++)
			{
				ostringstream ss;
				ss << "pointLights[" << a << "]";
				string str = ss.str();

				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".position").c_str()), -pointLight[a]->x, pointLight[a]->y, pointLight[a]->z);

				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".constant").c_str()), pointLight[a]->constant);
				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".linear").c_str()), pointLight[a]->linear);
				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".quadratic").c_str()), pointLight[a]->quadratic);

				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".ambient").c_str()), pointLight[a]->ambient.x, pointLight[a]->ambient.y, pointLight[a]->ambient.z);
				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".diffuse").c_str()), pointLight[a]->diffuse.x*pointLight[a]->intensity, pointLight[a]->diffuse.y*pointLight[a]->intensity, pointLight[a]->diffuse.z*pointLight[a]->intensity);
				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".specular").c_str()), pointLight[a]->specular.x, pointLight[a]->specular.y, pointLight[a]->specular.z);

				if(a >= maxPointLight-1)
					break;
			}

			if(spotLight.size() < maxSpotLight)
				glUniform1i(glGetUniformLocation(materialList[i]->program, "countSpotLight"), spotLight.size());
			else
				glUniform1i(glGetUniformLocation(materialList[i]->program, "countSpotLight"), maxSpotLight);

			for(GLint a = 0; a < spotLight.size(); a++)
			{
				ostringstream ss;
				ss << "spotLights[" << a << "]";
				string str = ss.str();

				spotLight[a]->direction = glm::vec3(cosf(-spotLight[a]->rotationX*(MATH_PI/180.0f)) * sinf(-spotLight[a]->rotationY*(MATH_PI/180.0f)), sinf(-spotLight[a]->rotationX*(MATH_PI/180.0f)), cosf(-spotLight[a]->rotationX*(MATH_PI/180.0f)) * cosf(-spotLight[a]->rotationY*(MATH_PI/180.0f)));

				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".position").c_str()), spotLight[a]->x, spotLight[a]->y, spotLight[a]->z);
				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".direction").c_str()), spotLight[a]->direction.x, spotLight[a]->direction.y, spotLight[a]->direction.z);

				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".constant").c_str()), spotLight[a]->constant);
				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".linear").c_str()), spotLight[a]->linear);
				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".quadratic").c_str()), spotLight[a]->quadratic);

				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".cutOff").c_str()), glm::cos(glm::radians(spotLight[a]->cutOff)));
				glUniform1f(glGetUniformLocation(materialList[i]->program, (str+".outerCutOff").c_str()), glm::cos(glm::radians(spotLight[a]->outerCutOff)));

				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".ambient").c_str()), spotLight[a]->ambient.x, spotLight[a]->ambient.y, spotLight[a]->ambient.z);
				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".diffuse").c_str()), spotLight[a]->diffuse.x*spotLight[a]->intensity, spotLight[a]->diffuse.y*spotLight[a]->intensity, spotLight[a]->diffuse.z*spotLight[a]->intensity);
				glUniform3f(glGetUniformLocation(materialList[i]->program, (str+".specular").c_str()), spotLight[a]->specular.x, spotLight[a]->specular.y, spotLight[a]->specular.z);

				if(a >= maxSpotLight-1)
					break;
			}

			glUniform3f(glGetUniformLocation(materialList[i]->program, "viewPos"), mainCamera->x, mainCamera->y, mainCamera->z);
		}

		glUniformMatrix4fv(glGetUniformLocation(materialList[i]->program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
		glUniformMatrix4fv(glGetUniformLocation(materialList[i]->program, "view"), 1, GL_FALSE, glm::value_ptr(_view));

		for(GLint a = 0; a < objectList.size(); a++)
		{
			if(objectList[a]->GetMaterial() == materialList[i])
				objectList[a]->Update(mainCamera);
		}
	}
}

void Scene::SetScreen(GLfloat width, GLfloat height)
{
	screenWidth = width;
	screenHeight = height;
}

void Scene::SetGamma(GLfloat volume)
{
	gamma = volume;	
}