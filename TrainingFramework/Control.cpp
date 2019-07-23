#include "stdafx.h"
#include "Control.h"

Control::Control()
{
	type = CONTROL;
	inputMan = InputManager::GetInstance();
}

Control::~Control()
{
}

void Control::Update(float deltaTime)
{
	InputManager * inputMan = InputManager::GetInstance();

	//Movement
	//
	Vector2 moveVector;
	Vector2 zero = Vector2();
	moveVector = (ActionCheck(MoveForward) ? Vector2(0, 1) : zero)
		+ (ActionCheck(MoveBackWard) ? Vector2(0, -1) : zero)
		+ (ActionCheck(MoveLeft) ? Vector2(-1, 0) : zero)
		+ (ActionCheck(MoveRight) ? Vector2(1, 0) : zero);
	Vector2 moveOffset = moveVector * speed * deltaTime;
	Transform * transform = (Transform*)parentTrans;
	transform->addToPosition(moveOffset.x, moveOffset.y);
	cout << transform->position.x << " " << transform->position.y << endl;
	//Beat It
}

bool Control::ActionCheck(Actions action)
{
	return inputMan->ActionCheck(action);
}
