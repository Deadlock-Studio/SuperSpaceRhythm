#pragma once
#include "InputManager.h"
class Control :
	public Component
{
public:
	Control();
	~Control();

	void Update(float deltaTime) override;
	bool ActionCheck(Actions action);

	float speed = 100.0f;

	InputManager * inputMan = NULL;
};

