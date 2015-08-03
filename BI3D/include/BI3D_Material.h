#ifndef BI3D_MATERIAL_INCLUDE_H
#define BI3D_MATERIAL_INCLUDE_H

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#define BI3D_DEFAULT 0
#define BI3D_SUPPORT_LIGHT 1

using namespace std;

class Material
{
public:
	GLuint program;
	
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