#pragma once
#include "GameObject.h"
#include "Animation.h"
class Mob1 :
	public GameObject
{
public:
	Mob1();
	Mob1(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Mob1();

	void AddComponent(Component* comp);
	void SetState(void(Mob1::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void CalculateVelocity();
	float velX, velY;
	int status = 0;
	void(Mob1::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};