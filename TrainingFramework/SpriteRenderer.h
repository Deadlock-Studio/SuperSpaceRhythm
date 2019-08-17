#pragma once
#include "Component.h"
#include "../Utilities/utilities.h"

class SpriteRenderer :
	public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(char * filename, int pixelPerUnit);
	~SpriteRenderer();

	void Draw() override;
	void LateUpdate(float deltaTime);
	Component * Clone() override;
	void FadeOff();
	void FadeIn(float fadeSpeed);
	void FadeOut(float fadeSpeed);

	float fadeIn = 0;
	float fadeOut = 0;
	Matrix scaleMatrix;
	int textureId = -1;
	int pixelsPerUnit;
	float alpha = 1;
};

