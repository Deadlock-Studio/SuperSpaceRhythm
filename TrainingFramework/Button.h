#pragma once
#include "Component.h"
class Button :
	public Component
{
public:
	Button();
	~Button();

	void Init();
	Component* Clone();
	void Update(float deltaTime);

	bool isClicked = false;
	bool isHovered = false;
	Texture* sprite = NULL;
};

