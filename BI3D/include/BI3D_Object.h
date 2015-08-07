#ifndef BI3D_Object_INCLUDE_H
#define BI3D_Object_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <SDL_image.h>

#include "BI3D_Camera.h"
#include "BI3D_Material.h"
#include "BI3D_Mesh.h"

#define MATH_PI 3.14159265359f
#define BI3D_TEXTURE_DIFFUSE 0
#define BI3D_TEXTURE_SPECULAR 1
#define BI3D_TEXTURE_NORMAL 2

using namespace std;

unsigned int LoadImage(string path);

class Object
{
public:
	GLfloat x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ;

	Object();
	~Object();

	void LoadTexture(string path, int type = BI3D_TEXTURE_DIFFUSE);
	void Load(string path, int mode = BI3D_LOAD_DEFAULT);
	void LoadPrefab(Object* ob);
	void LookAt(GLfloat x, GLfloat y, GLfloat z);
	void SetMaterialMode(int mode);
	void SetShininess(GLfloat shininess);

	void Update(Camera* camera);

	Material* GetMaterial();
	vector<Mesh*> GetMesh();
	string GetDirectory();
	bool GetClone();
	int GetMode();
private:
	Material* material;
	vector<Mesh*> meshList;
	string directory;

	int mode;
	bool clone;

	vector<Texture> _diffuseMaps;
	vector<Texture> _specularMaps;
	vector<Texture> _normalMaps;

	bool autoLoadTexture;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif