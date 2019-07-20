#pragma once
#include "../Utilities/utilities.h"

class Texture
{
public:
	Texture();
	Texture(char* filename);
	~Texture();

	GLuint texId;
	int width, height, bpp;
	char * filename;
};

