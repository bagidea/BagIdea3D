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

using namespace std;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
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
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vec, vector<GLuint> ind, vector<Texture> tex);

	void Update(Material* material, glm::mat4 _projection, glm::mat4 _view, glm::mat4 _model);
private:
	GLuint VAO, VBO, EBO;

	void SetupMesh();
};

#endif