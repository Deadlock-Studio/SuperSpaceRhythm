#include <stdafx.h>
#include "Shaders.h"
#include <iostream>

Shaders::Shaders()
{
}

//All in one shaders class
Shaders::Shaders(char * fileVertexShader, char * fileFragmentShader)
{
	verFile = fileVertexShader;
	fragFile = fileFragmentShader;

	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (vertexShader == 0) {
		return;
	}
		
	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	/*
	* Finding attributes and uniforms
	*/
	//Common
	positionLoc = glGetAttribLocation(program, "a_posL");
	mvpLoc = glGetUniformLocation(program, "u_mvp");

	//Texture2D
	uvLoc = glGetAttribLocation(program, "a_uv");
	textureLoc = glGetUniformLocation(program, "u_texture");
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}