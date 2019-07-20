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

	int pixelsPerUnit = 100;
private:
	Matrix scaleMatrix;
	int textureId = -1;
	int modelId;
};

