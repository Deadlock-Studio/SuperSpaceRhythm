#pragma once
#include <vector>
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Camera.h"
#include "CubeTexture.h"
using namespace std;

class Object
{
public:
	Object();
	~Object();
	void Print();
	void Draw(Camera * camera);
	void setPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; }
	void setRotation(float x, float y, float z) { rotation.x = x; rotation.y = y; rotation.z = z; }
	void setScale(float x, float y, float z) { scale.x = x; scale.y = y; scale.z = z; }
	void initMatrix();

	int modelID = -1;
	vector<int> textureID {};
	vector<int> cubeTextureID {};
	int shadersID = -1;

	Matrix m_model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

