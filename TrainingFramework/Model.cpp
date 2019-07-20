#include "stdafx.h"
#include "Model.h"
#include "HandyString.h"
#include "ResourceManager.h"
#include <iostream>

Model::Model()
{
}

Model::Model(char * filename)
{
	FILE * f;
	f = fopen(filename, "r");
	this->filename = filename;

	if (f) {
		fscanf(f, "NrVertices: %d\n", &nVertices);
		Vertex * verticesList = new Vertex[nVertices];
		Vertex ver;
		Vector3 offset; Vector3 zero = Vector3(0, 0, 0);
		for (int i = 0; i < nVertices; i++) {
			offset = zero;

			// 0. pos:[0.134000, 1.020300, -0.083900];
			//uv:[0.611900, 0.886700];
			fscanf(f, "%*d. ");
			float x, y, z;
			fscanf(f, "pos:[%f, %f, %f]; ", &x, &y, &z);
			ver.pos.x = x + offset.x; ver.pos.y = y + offset.y; ver.pos.z = z + offset.z;
			fscanf(f, "uv:[%f, %f];\n", &x, &y);
			ver.uv.x = x; ver.uv.y = y;

			verticesList[i] = ver;
		}

		fscanf(f, "NrIndices: %d\n", &nIndices);

		int * indicesList = new int[nIndices];

		for (int i = 0; i < nIndices /3; i++) {
			fscanf(f, "%*d. ");
			int x, y, z;
			fscanf(f, "%d, %d, %d\n", &x, &y, &z);
			indicesList[i * 3] = x;
			indicesList[i * 3 + 1] = y;
			indicesList[i * 3 + 2] = z;
		}

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nVertices, verticesList, GL_STATIC_DRAW);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*nIndices, indicesList, GL_STATIC_DRAW);

		delete[] verticesList;
		delete[] indicesList;
	}
	else {
		printf("Model file not available\n");
	}
}


Model::~Model()
{

}