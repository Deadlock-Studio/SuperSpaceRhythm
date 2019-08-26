#pragma once
#include "InputManager.h"
#define SPD_BOOST_TIME 120

class Control :
	public Component
{
public:
	Control();
	~Control();

	void Update(float deltaTime) override;
	Component * Clone() override;
	bool ActionCheck(Actions action);
	void Collide(float x, float y, bool isPlayerTouching, char * type);
	float speed = MOVE_SPEED;
	InputManager * inputMan = NULL;
	int dashTick = 0;
	int colTick = 0;
	float dashSpeed;
	float colSpeed;
	Vector2 dashVec;
	b2Vec2 colVec;
	bool disableControl = FALSE;
	int spdBoostCounter = 0;

	bool isDashing = FALSE;
	bool isColliding = FALSE;
	bool isReforming = FALSE;

	bool boss = FALSE;
	bool trap = FALSE;
};

