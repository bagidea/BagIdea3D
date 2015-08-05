#ifndef BI3D_MESH_INCLUDE_H
#define BI3D_MESH_INCLUDE_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BI3D_Material.h"

#define BI3D_LOAD_DEFAULT 0
#define BI3D_LOAD_NORMALMAP 1

using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct Vertex2
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<Vertex2> vertices2;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vec, vector<GLuint> ind, vector<Texture> tex);
	Mesh(vector<Vertex2> vec, vector<GLuint> ind, vector<Texture> tex);

	void Update(Material* material);
private:
	GLuint VAO, VBO, EBO;

	int mode;

	void SetupMesh();
};

#endif