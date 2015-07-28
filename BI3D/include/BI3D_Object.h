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

using namespace std;

unsigned int LoadImage(string path);

class Object
{
public:
	GLfloat x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ;

	Object();
	~Object();

	void Load(string path);
	void LoadPrefab(Object* ob);
	void LookAt(GLfloat x, GLfloat y, GLfloat z);

	void Update(Camera* camera);

	Material* GetMaterial();
	vector<Mesh*> GetMesh();
	string GetDirectory();
	bool GetClone();
private:
	Material* material;
	vector<Mesh*> meshList;
	string directory;

	bool clone;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif