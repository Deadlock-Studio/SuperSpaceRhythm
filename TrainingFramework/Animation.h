#pragma once
#include"Model.h"
#include"Texture.h"
#include<vector>
#include<ctime>

class Animation :
	public Component
{
public:
	Animation();
	Animation(char * filename, float spriteX, float spriteY, float spriteW, float spriteH, int nSprite, float animSpeed, int pixelsPerUnit);
	~Animation();

	void Draw() override;
	void Update(float deltaTime) override;
	Component * Clone() override;
	void RunAnimation(float deltaTime);

	Matrix scaleMatrix;
	int textureId = -1;
	int pixelsPerUnit;

	float tick = 0;
	int nSprite = 0;
	int activeSprite = 0;
	vector<GLuint> vboId{};
	float spriteX, spriteY, spriteW, spriteH;
	float animSpeed;
	float alpha = 1;
};


