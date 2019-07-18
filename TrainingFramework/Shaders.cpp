#include <stdafx.h>
#include "Shaders.h"
#include <iostream>

Shaders::Shaders()
{
}

//All in one shaders class
Shaders::Shaders(char * fileVertexShader, char * fileFragmentShader)
{
	///Vertex
	verFile = fileVertexShader;
	fragFile = fileFragmentShader;
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (vertexShader == 0) {
		return;
	}
		

	///Fragment
	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

		//Finding attribute/uniform
	//Common
	positionLoc = glGetAttribLocation(program, "a_posL");
	mvpLoc = glGetUniformLocation(program, "u_mvp");
	m_model = glGetUniformLocation(program, "m_model");

	//Texture2D
	uvLoc = glGetAttribLocation(program, "a_uv");
	textureLoc = glGetUniformLocation(program, "u_texture");
	
	//CubeTexture
	u_cubeTexture = glGetUniformLocation(program, "u_cubeTexture");

	//Terrain
	u_blendmap = glGetUniformLocation(program, "u_blendmap");
	u_rock = glGetUniformLocation(program, "u_rock");
	u_dirt = glGetUniformLocation(program, "u_dirt");
	u_grass = glGetUniformLocation(program, "u_grass");

	//Fog
	u_cam = glGetUniformLocation(program, "u_cam");
	u_fogStart = glGetUniformLocation(program, "u_fogStart");
	u_fogRange = glGetUniformLocation(program, "u_fogRange");
	u_fogColor = glGetUniformLocation(program, "u_fogColor");

	//Fire
	u_time = glGetUniformLocation(program, "u_time");
	u_dismap = glGetUniformLocation(program, "u_dismap");
	u_mask = glGetUniformLocation(program, "u_mask");
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}