#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char * verFile;
	char * fragFile;
	//Common
	GLint positionLoc;
	GLint mvpLoc;
	GLint m_model;

	//Texture2D
	GLint uvLoc;
	GLint textureLoc;

	//CubeTexture
	GLint u_cubeTexture;

	//Terrain
	GLint u_blendmap;
	GLint u_rock;
	GLint u_dirt;
	GLint u_grass;

	//Fog
	GLint u_cam;
	GLint u_fogStart;
	GLint u_fogRange;
	GLint u_fogColor;

	//Fire
	GLint u_time;
	GLint u_dismap;
	GLint u_mask;

	Shaders();
	Shaders(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};