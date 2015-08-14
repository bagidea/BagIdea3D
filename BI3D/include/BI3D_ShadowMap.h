#ifndef BI3D_SHADOWMAP_INCLUDE_H
#define BI3D_SHADOWMAP_INCLUDE_H

#include "BI3D_FrameBuffer.h"

class ShadowMap
{
public:
	ShadowMap(int width, int height);
	~ShadowMap();

	void Bind();

	GLuint GetFrameBuffer();
	unsigned int GetTexture();
private:
	int width, height;

	FrameBuffer* shadowMap;
};

#endif