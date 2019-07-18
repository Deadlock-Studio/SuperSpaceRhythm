#include "stdafx.h"
#include "CubeTexture.h"
#include <iostream>

CubeTexture::CubeTexture()
{
}

CubeTexture::CubeTexture(char * parentName)
{
	this->parentName = parentName;
	for (int i = 0; i < 6; i++) {
		filename[i] = NULL;
	}
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


CubeTexture::~CubeTexture()
{
}

bool CubeTexture::AddTexture(char * filename, int pos)
{
	if (this->filename[pos] == NULL) {
		this->filename[pos] = filename;
		Update();
		return true;
	}
	else std::cout << "Duplicate side detected in cube texture.\n";
	return false;
}

void CubeTexture::SetTexture(char * filename, int pos)
{
	this->filename[pos] = filename;
	Update();
}
//Update the texture in GPU
void CubeTexture::Update()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	for (int i = 0; i < 6; i++)
	{
		if (filename[i] == NULL) continue;
		int width, height, bpp;
		char * imageData = LoadTGA(filename[i], &width, &height, &bpp);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,imageData);
		if (imageData) delete imageData;
	}
}
