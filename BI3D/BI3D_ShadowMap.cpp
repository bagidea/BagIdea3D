#include "BI3D_ShadowMap.h"

ShadowMap::ShadowMap(int width, int height)
{
	this->width = width;
	this->height = height;

	shadowMap = new FrameBuffer(this->width, this->height, true);
}

ShadowMap::~ShadowMap()
{
	delete shadowMap;
	shadowMap = NULL;
}

void ShadowMap::Bind()
{
	shadowMap->Bind();
}

GLuint ShadowMap::GetFrameBuffer(){return shadowMap->GetFrameBuffer();}
unsigned int ShadowMap::GetTexture(){return shadowMap->GetTexture();}