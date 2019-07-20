#pragma once

enum ComponentType {
	TRANSFORM,
	SPRITE,
};

class Component
{
public:
	Component();
	~Component();

	virtual void Update() {};
	virtual void Draw() {};
	ComponentType type;
	void * parentObj = NULL;
};

