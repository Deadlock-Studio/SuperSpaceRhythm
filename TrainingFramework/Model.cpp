#include "stdafx.h"
#include "Model.h"
#include "HandyString.h"
#include "ResourceManager.h"
#include <iostream>

Model::Model()
{
}

float height(float x, float z) {
	ResourceManager * resource = ResourceManager::GetInstance();

	int mapX = (int)(x + 64);
	int mapZ = (int)(z + 64);

	int mapPoint = 0;
	float heightValue = 0;
	
	if (mapX > 127) mapX = 127;
	if (mapZ > 127) mapZ = 127;

	return ((unsigned int)resource->heightmap[mapX + mapZ * 128] / (float)255) * resource->height;
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
			//norm:[0.662725, 0.317712, -0.678126];
			//binorm:[0.014559, 0.899903, 0.435847];
			//tgt:[-0.748721, 0.298719, -0.591763];
			//uv:[0.611900, 0.886700];
			fscanf(f, "%*d. ");
			float x, y, z;
			fscanf(f, "pos:[%f, %f, %f]; ", &x, &y, &z);
			if (!HandyString::getFileName(string(filename)).compare("Terrain")) {
				offset.y += height(x, z);
			}
			ver.pos.x = x + offset.x; ver.pos.y = y + offset.y; ver.pos.z = z + offset.z;
			fscanf(f, "norm:[%f, %f, %f]; ", &x, &y, &z);
			ver.normal.x = x; ver.normal.y = y; ver.normal.z = z;
			fscanf(f, "binorm:[%f, %f, %f]; ", &x, &y, &z);
			ver.binormal.x = x; ver.binormal.y = y; ver.binormal.z = z;
			fscanf(f, "tgt:[%f, %f, %f]; ", &x, &y, &z);
			ver.tangent.x = x; ver.tangent.y = y; ver.tangent.z = z;
			fscanf(f, "uv:[%f, %f]; \n", &x, &y);
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