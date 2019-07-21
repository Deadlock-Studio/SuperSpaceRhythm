#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	Shaders();
	Shaders(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();

	GLuint program, vertexShader, fragmentShader;
	char * verFile;
	char * fragFile;

	//Common
	GLint positionLoc;
	GLint mvpLoc;

	//Texture2D
	GLint uvLoc;
	GLint textureLoc;
};