#pragma once
#include "../Utilities/utilities.h"

class Texture
{
public:
	Texture();
	Texture(char* filename);
	~Texture();

	GLuint texId;
	char * filename;
};

