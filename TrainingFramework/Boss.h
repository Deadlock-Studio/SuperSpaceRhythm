#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Control.h"
class Boss :
	public GameObject
{
public:
	Boss();
	Boss(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Boss();

	void AddComponent(Component* comp);
	void SetState(void(Boss::*state)()) {
		activeState = state;
	}
	float x, y, velX, velY, distance;

	void PlayAnimation(int key);
	void Idle();
	void Spawn();

	void WalkLeft();
	void WalkRight();
	void Death();
	void CalculateVelocity(float mX, float mY);
	void DoStuff();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void(Boss::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};