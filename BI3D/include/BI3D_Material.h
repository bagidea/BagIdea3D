#ifndef BI3D_MATERIAL_INCLUDE_H
#define BI3D_MATERIAL_INCLUDE_H

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

using namespace std;

class Material
{
public:
	GLuint program;
	
	Material(string vertexShaderPath, string fragmentShaderPath);
	~Material();

	void Bind();
private:
	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint CompileShader(string source, unsigned int type);
	string LoadSource(string path);
};

#endif