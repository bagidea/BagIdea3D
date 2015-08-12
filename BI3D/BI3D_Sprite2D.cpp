#include "BI3D_Sprite2D.h"

Sprite2D::Sprite2D()
{
    FlipVertical = false;

	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;
	width = 0.0f;
	height = 0.0f;

	color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	Init();
}

void Sprite2D::Load(string path)
{	
	int w, h;

	tex = LoadImage(path, w, h);

	width = (GLfloat)w;
	height = (GLfloat)h;
}

void Sprite2D::SetTexture(unsigned int tex)
{
    this->tex = tex;
}

void Sprite2D::Update(Material* material)
{
	glm::mat4 model;

    model = glm::translate(model, glm::vec3(x, y, 0.0f));  

    model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));

    model = glm::scale(model, glm::vec3(width, height, 1.0f)); 

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniformMatrix4fv(material->gModel, 1, GL_FALSE, glm::value_ptr(model));
    
	glUniform1i(glGetUniformLocation(material->program, "image"), 0);
	glUniform4f(glGetUniformLocation(material->program, "baseColor"), color.r, color.g, color.b, color.a);
    glUniform1i(glGetUniformLocation(material->program, "flipVertical"), FlipVertical);

	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite2D::Init()
{
    GLfloat vertices[] = { 
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    
    glBindVertexArray(0);
}