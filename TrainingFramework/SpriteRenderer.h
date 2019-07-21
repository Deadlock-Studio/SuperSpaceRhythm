#pragma once
#include "Component.h"
#include "../Utilities/utilities.h"

class SpriteRenderer :
	public Component
{
public:
	SpriteRenderer();
	SpriteRenderer(char * filename);
	~SpriteRenderer();

	void Draw() override;

	Matrix scaleMatrix;
	int textureId = -1;
	int pixelsPerUnit = 100;
};

