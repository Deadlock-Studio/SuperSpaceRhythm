#pragma once
#include"Model.h"
#include"Texture.h"
#include<ctime>

class Animation :
	public Component
{
public:
	Animation();
	Animation(char * filename, float spriteX, float spriteY, float spriteW, float spriteH, float textureW, float textureH);
	~Animation();

	void Draw() override;
	void Update(float deltaTime);
	void RunAnimation(float deltaTime);

	Matrix scaleMatrix;
	int textureId = -1;
	int pixelsPerUnit = 1000;
	Model * a_model;
	Texture * text;
	GLuint a_iboId;
	GLuint a_vboId;
	Vertex verticesList[4];
	int tick = 0;
	float spriteX, spriteY, spriteW, spriteH, textureW, textureH;
	float a_W, a_H, a_u, a_v;

};


