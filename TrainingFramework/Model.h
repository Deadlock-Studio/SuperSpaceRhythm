#pragma once
#include <vector>
#include "Vertex.h"

class Model
{
public:
	Model();
	Model(char * filename);
	~Model();

	GLuint vboId, iboId;
	unsigned int nVertices, nIndices;
	char * filename;
};

