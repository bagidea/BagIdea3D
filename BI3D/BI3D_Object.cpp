#include "BI3D_Object.h"

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

Object::Object()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;
	scaleX = 1.0f;
	scaleY = 1.0f;
	scaleZ = 1.0f;

	directory = "";

	material = new Material("shader/Default.vs", "shader/Default.fs", BI3D_DEFAULT);

	clone = false;
	autoLoadTexture = true;
}

Object::~Object()
{
	for(GLint i = 0; i < meshList.size(); i++)
	{
		if(meshList[i] != NULL)
		{
			if(!clone)
			{
				delete meshList[i];
				meshList[i] = NULL;
			}
		}
	}

	meshList.clear();

	if(material != NULL)
	{
		delete material;
		material = NULL;
	}
}

void Object::LoadTexture(string path, int type)
{
	int w, h;

	Texture texture;
	texture.id = LoadImage(path, w ,h);
	texture.path = "";

	if(type == BI3D_TEXTURE_DIFFUSE)
	{
		texture.type = "texture_diffuse";
		_diffuseMaps.push_back(texture);
	}
	else if(type == BI3D_TEXTURE_SPECULAR)
	{
		texture.type = "texture_specular";
		_specularMaps.push_back(texture);
	}
	else if(type == BI3D_TEXTURE_NORMAL)
	{
		texture.type = "texture_normal";
		_normalMaps.push_back(texture);
	}

	autoLoadTexture = false;
}

void Object::Load(string path)
{
	Assimp::Importer importer;

	GLuint flag;
	
	//const aiScene* scene = importer.ReadFile(path, flag);
	const aiScene* scene;

	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

	clone = false;
}

void Object::LoadPrefab(Object* ob)
{
	x = ob->x;
	y = ob->y;
	z = ob->z;
	rotationX = ob->rotationX;
	rotationY = ob->rotationY;
	rotationZ = ob->rotationZ;
	scaleX = ob->scaleX;
	scaleY = ob->scaleY;
	scaleZ = ob->scaleZ;

	material = ob->GetMaterial();
	meshList = ob->GetMesh();
	directory = ob->GetDirectory();

	clone = true;
}

void Object::LookAt(GLfloat x, GLfloat y, GLfloat z)
{
	glm::vec3 Pos(-this->x, this->y, this->z);
	glm::vec3 Target(x, y, z);

	glm::mat4 m = glm::lookAt(Pos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::quat Rotate = glm::conjugate(glm::quat_cast(m));
	glm::vec3 v = glm::eulerAngles(Rotate);

	if(Pos.z > Target.z)
		rotationX = glm::degrees(-v.x);
	else
		rotationX = glm::degrees(-v.x)+180.0f;

	if(Pos.z > Target.z)
		if(Pos.x > Target.x)
			rotationY = glm::degrees(-v.y)-180.0f;
		else
			rotationY = glm::degrees(-v.y)+180.0f;
	else
		rotationY = glm::degrees(v.y);

	rotationZ = glm::degrees(0.0f);
}

void Object::SetMaterialMode(int mode)
{
	if(material != NULL)
	{
		delete material;
		material = NULL;
	}

	if(mode == BI3D_DEFAULT)
		material = new Material("shader/Default.vs", "shader/Default.fs", mode);
	else if(mode == BI3D_SUPPORT_LIGHT)
		material = new Material("shader/SupportLight.vs", "shader/SupportLight.fs", mode);
	else if(mode == BI3D_SUPPORT_LIGHT_AND_NORMALMAP)
		material = new Material("shader/SupportLightAndNormalMap.vs", "shader/SupportLightAndNormalMap.fs", mode);
}

void Object::SetShininess(GLfloat shininess)
{
	material->shininess = shininess;
}

void Object::Update(Camera* camera)
{
	glm::mat4 _model;

	if(camera != NULL)
	{
		glm::mat4 position = glm::translate(glm::mat4(), glm::vec3(-x, y, z));
		glm::mat4 roX = glm::rotate(glm::mat4(), glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 roY = glm::rotate(glm::mat4(), glm::radians(-rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 roZ = glm::rotate(glm::mat4(), glm::radians(-rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotation = roY * roX * roZ;
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(scaleX, scaleY, scaleZ));
		_model = position * rotation * scale;

		glUniformMatrix4fv(material->gModel, 1, GL_FALSE, glm::value_ptr(_model));

		for(GLint i = 0; i < meshList.size(); i++)
			meshList[i]->Update(material);
	}
}

Material* Object::GetMaterial(){return material;}
vector<Mesh*> Object::GetMesh(){return meshList;}
string Object::GetDirectory(){return directory;}
bool Object::GetClone(){return clone;}

void Object::ProcessNode(aiNode* node, const aiScene* scene)
{
	for(GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
		this->meshList.push_back(ProcessMesh(mesh, scene));			
	}

	for(GLuint i = 0; i < node->mNumChildren; i++)
	{
 		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Object::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for(GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

	 	vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
	    vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if(mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}else{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for(GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for(GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if(autoLoadTexture)
	{
		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		}
	}else{
		textures.insert(textures.end(), _diffuseMaps.begin(), _diffuseMaps.end());
		textures.insert(textures.end(), _specularMaps.begin(), _specularMaps.end());
		textures.insert(textures.end(), _normalMaps.begin(), _normalMaps.end());
	}

	return new Mesh(vertices, indices, textures);
}

vector<Texture> Object::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	vector<Texture> textureList;
	
	for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;

		for(GLuint a = 0; a < textureList.size(); a++)
		{
			if(textureList[a].path == str)
			{
				textures.push_back(textureList[a]);
				skip = true;
				break;
			}
		}

		if(!skip)
		{
			int x, y;

			Texture texture;
			texture.id = LoadImage(directory+"/"+str.C_Str(), x, y);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textureList.push_back(texture);
		}
	}

	return textures;
}

unsigned int LoadImage(string path, int& w_, int& h_)
{
	SDL_Surface* img = IMG_Load(path.c_str());
	unsigned int id;

	if(img == NULL)
	{
		cout << "Can't Load Image : " << path << endl;
	}else{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		GLenum formatTexture;
		switch(img->format->BytesPerPixel)
		{
		case 4:
			if(img->format->Rmask == 0x000000ff)
			{
				formatTexture = GL_RGBA;
			}else{
				formatTexture = GL_BGRA;
			}
			break;
		case 3:
			if(img->format->Rmask == 0x000000ff)
			{
				formatTexture = GL_RGB;
			}else{
				formatTexture = GL_BGR;
			}
			break;
		default:
			cout << "The Image not Truecolor." << endl;
		}


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, formatTexture, img->w, img->h, 0, formatTexture, GL_UNSIGNED_BYTE, img->pixels);

		w_ = img->w;
		h_ = img->h;

		SDL_FreeSurface(img);
		img = NULL;
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return id;
}