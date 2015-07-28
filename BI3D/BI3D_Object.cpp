#include "BI3D_Object.h"

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

	material = new Material("shader/Default.vs", "shader/Default.fs");

	clone = false;
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

void Object::Load(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

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

void Object::Update(Camera* camera)
{
	glm::mat4 _model;

	if(rotationX >= 360.0f)
		rotationX = 360.0f;
	else if(rotationX <= -360.0f)
		rotationX = -360.0f;

	if(rotationY >= 360.0f)
		rotationY = 360.0f;
	else if(rotationY <= -360.0f)
		rotationY = -360.0f;

	if(rotationZ >= 360.0f)
		rotationZ = 360.0f;
	else if(rotationZ <= -360.0f)
		rotationZ = -360.0f;

	if(rotationX >= 180.0f)
		rotationX = -180.0f+(rotationX-180.0f);
	else if(rotationX <= -180.0f)
		rotationX = 180.0f+(rotationX+180.0f);

	if(rotationY >= 180.0f)
		rotationY = -180.0f+(rotationY-180.0f);
	else if(rotationY <= -180.0f)
		rotationY = 180.0f+(rotationY+180.0f);

	if(rotationZ >= 180.0f)
		rotationZ = -180.0f+(rotationZ-180.0f);
	else if(rotationZ <= -180.0f)
		rotationZ = 180.0f+(rotationZ+180.0f);

	if(camera != NULL)
	{
		glm::mat4 position = glm::translate(glm::mat4(), glm::vec3(-x, y, z));

		glm::vec3 vec();
		//glm::quat quat(glm::vec3(glm::radians(rotationX), glm::radians(-rotationY), glm::radians(-rotationZ)));
		//glm::mat4 rotation = glm::mat4_cast(quat);
		glm::mat4 roX = glm::rotate(glm::mat4(), glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 roY = glm::rotate(glm::mat4(), glm::radians(-rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 roZ = glm::rotate(glm::mat4(), glm::radians(-rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 rotation = roY * roX * roZ;
		glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(scaleX, scaleY, scaleZ));
		_model = position * rotation * scale;

		glUniformMatrix4fv(glGetUniformLocation(material->program, "model"), 1, GL_FALSE, glm::value_ptr(_model));

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

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return new Mesh(vertices, indices, textures);
}

vector<Texture> Object::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	
	for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		Texture texture;
		texture.id = LoadImage(directory+"/"+str.C_Str());
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
	}

	return textures;
}

unsigned int LoadImage(string path)
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

		SDL_FreeSurface(img);
		img = NULL;
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return id;
}