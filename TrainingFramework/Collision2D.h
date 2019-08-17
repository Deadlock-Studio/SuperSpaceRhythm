#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Box2D\Box2D.h"
class Collision2D :
	public Component
{
public:
	Collision2D();
	Collision2D(float width, float height,float mass,float res, char * type);
	~Collision2D();
	Component * Clone() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void addToPosition(float x, float y);
	void UpdateBodyPostition(float x, float y);
	void UpdateBodyScale(float x, float y);
	void UpdateBodyRotation(float angle);
	void Init();
	b2Body * body;
	char * bodyType;
	float bodyX;
	float bodyY;
	float bodyW;
	float bodyH;
	float bodyMass;
	float bodyRes;
	bool isBullet = false;
};

