#pragma once

enum ComponentType {
	SPRITE,
	CONTROL,
	ANIMATION,
};

class Component
{
public:
	Component();
	~Component();

	virtual void Update(float deltaTime) {};
	virtual void Draw() {};
	ComponentType type;
	void * parentObj = NULL;
	void * parentTrans = NULL;
};

