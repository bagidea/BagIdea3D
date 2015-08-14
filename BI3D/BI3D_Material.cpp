#include "BI3D_Material.h"

Material::Material(string vertexShaderPath, string fragmentShaderPath, int type)
{
	this->type = type;
	this->shininess = 64.0f;

	program = glCreateProgram();

	vertexShader = CompileShader(LoadSource(vertexShaderPath), GL_VERTEX_SHADER);
	fragmentShader = CompileShader(LoadSource(fragmentShaderPath), GL_FRAGMENT_SHADER);

	if(vertexShader == NULL || fragmentShader == NULL)
	{
		cerr << "Craete Shader Fail" << endl;
	}else{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		
		if(!success)
		{
			GLchar error[1024];
			glGetProgramInfoLog(program, 1024, NULL, error);
			cout << error << endl;	
		}
	}

	gProjection = glGetUniformLocation(program, "projection");
	gView = glGetUniformLocation(program, "view");
	gModel = glGetUniformLocation(program, "model");

	gShininess = glGetUniformLocation(program, "material.shininess");

	gDirectionalLight_Direction = glGetUniformLocation(program, "directionalLight.direction");
	gDirectionalLight_Ambient = glGetUniformLocation(program, "directionalLight.ambient");
	gDirectionalLight_Diffuse = glGetUniformLocation(program, "directionalLight.diffuse");
	gDirectionalLight_Specular = glGetUniformLocation(program, "directionalLight.specular");

	gShadowMap = glGetUniformLocation(program, "lightSpace");

	gPointLight_Count = glGetUniformLocation(program, "countPointLight");
	gSpotLight_Count = glGetUniformLocation(program, "countSpotLight");

	gViewPos = glGetUniformLocation(program, "viewPos");
}

Material::~Material()
{
	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

void Material::Bind()
{
	glUseProgram(program);
}

int Material::GetType()
{
	return type;
}

GLuint Material::CompileShader(string source, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	const char* sourceText[1];
	sourceText[0] = source.c_str();

	GLint length[1];
	length[0] = source.length();

	glShaderSource(shader, 1, sourceText, length);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		GLchar error[1024];
		glGetShaderInfoLog(shader, 1024, NULL, error);
		cout << error << endl;
	}

	return shader;
}

string Material::LoadSource(string path)
{
	string line, output;
	output = "";

	ifstream in(path.c_str());
	if(!in.is_open())
	{
		cerr << "Load Shader Fail" << endl;
		return "";
	}else{
		while(in.good())
		{
			getline(in, line);
			output.append(line+"\n");
		}
	}

	return output;
}