#ifndef BI3D_FRAMEBUFFER_INCLUDE_H
#define BI3D_FRAMEBUFFER_INCLUDE_H

#include <iostream>

#include <GL/glew.h>

using namespace std;

class FrameBuffer
{
public:
	FrameBuffer(int width, int height, GLboolean depth = false, GLboolean stencil = false);
	~FrameBuffer();

	void Bind();

	GLuint GetFrameBuffer();
	unsigned int GetTexture();
private:
	GLuint screenWidth, screenHeight;
	GLuint FBO, RBO;
	unsigned int textureColorBuffer;

	unsigned int GenAttachmentTexture(GLboolean depth, GLboolean stencil);
};

#endif