#ifndef BI3D_SPRITE2D_INCLUDE_H
#define BI3D_SPRITE2D_INCLUDE_H

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BI3D_Material.h"
#include "BI3D_Object.h"

using namespace std;

class Sprite2D
{
public:
	GLfloat x, y, rotation, width, height;
	Color color;

	GLboolean FlipVertical;

	Sprite2D();

	void Load(string path);
	void SetTexture(unsigned int tex);
	void Update(Material* material);
private:
	GLuint VAO, VBO;

	unsigned int tex;

	void Init();
};

#endif