#ifndef BI3D_MATERIAL_INCLUDE_H
#define BI3D_MATERIAL_INCLUDE_H

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#define BI3D_DEFAULT 0
#define BI3D_SUPPORT_LIGHT 1
#define BI3D_SUPPORT_LIGHT_AND_NORMALMAP 2
#define BI3D_SPRITE_2D 3

using namespace std;

class Material
{
public:
	GLuint program;
	GLfloat shininess;

	GLuint gShininess, gViewPos;
	GLuint gProjection, gView, gModel;

	GLuint gDirectionalLight_Direction, gDirectionalLight_Ambient, gDirectionalLight_Diffuse, gDirectionalLight_Specular;
	GLuint gPointLight_Count, gSpotLight_Count;

	Material(string vertexShaderPath, string fragmentShaderPath, int type);
	~Material();

	void Bind();
	int GetType();
private:
	int type;

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint CompileShader(string source, unsigned int type);
	string LoadSource(string path);
};

#endif