#pragma once
#include <iostream>

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};
	virtual void Draw() {};
	virtual Component * Clone() { return NULL; };
	bool isActive = true;
	void * parentObj = NULL;
	void * parentTrans = NULL;
};

