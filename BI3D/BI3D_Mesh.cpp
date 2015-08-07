#include "BI3D_Mesh.h"

Mesh::Mesh(vector<Vertex> vec, vector<GLuint> ind, vector<Texture> tex)
{
	vertices = vec;
	indices = ind;
	textures = tex;

	mode = BI3D_LOAD_DEFAULT;

	SetupMesh();
}

Mesh::Mesh(vector<Vertex2> vec, vector<GLuint> ind, vector<Texture> tex)
{
	vertices2 = vec;
	indices = ind;
	textures = tex;

	mode = BI3D_LOAD_NORMALMAP;

	SetupMesh();
}

void Mesh::Update(Material* material)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;

	for(GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0+i);

		stringstream ss;
		string number;
		string name = textures[i].type;

		if(name == "texture_diffuse")
		{
			ss << diffuseNr++;
		}
		else if(name == "texture_specular")
		{
			ss << specularNr++;
		}
		else if(name == "texture_normal")
		{
			ss << normalNr++;
		}

		number = ss.str();



		if(material->GetType() == BI3D_DEFAULT)		
			glUniform1i(glGetUniformLocation(material->program, (name+number).c_str()), i);
		else if(material->GetType() == BI3D_SUPPORT_LIGHT || material->GetType() == BI3D_SUPPORT_LIGHT_AND_NORMALMAP)	
			glUniform1i(glGetUniformLocation(material->program, ("material."+name+number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glUniform1f(glGetUniformLocation(material->program, "material.shininess"), material->shininess);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for(GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	if(mode == BI3D_LOAD_DEFAULT)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

		glBindVertexArray(0);
	}
	else if(mode == BI3D_LOAD_NORMALMAP)
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(Vertex2), &vertices2[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, texCoord));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, tangent));
		
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid*)offsetof(Vertex2, bitangent));

		glBindVertexArray(0);
	}
}