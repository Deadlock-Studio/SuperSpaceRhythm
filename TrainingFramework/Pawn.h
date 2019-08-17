#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Control.h"
class Pawn :
	public GameObject
{
public:
	Pawn();
	~Pawn();

	void AddComponent(Component* comp);
	void SetState(void(Pawn::*state)()) {
		activeState = state;
	}
	void Update(float deltaTime) {
		if (activeState != NULL)
			(this->*activeState)();
		GameObject::Update(deltaTime);
	}
	void PlayAnimation(int key);
	void Idle();
	void WalkLeft();
	void WalkRight();

	void(Pawn::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};