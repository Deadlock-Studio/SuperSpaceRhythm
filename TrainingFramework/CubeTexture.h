#pragma once
#include "../Utilities/utilities.h"

class CubeTexture
{
public:
	CubeTexture();
	CubeTexture(char * parentName);
	~CubeTexture();
	bool AddTexture(char * filename, int pos);
	void SetTexture(char * filename, int pos);
	void Update();

	GLuint texId;
	char * parentName;
	char * filename[6];
};

