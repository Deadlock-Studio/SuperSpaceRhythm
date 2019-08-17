#pragma once
#include "GameObject.h"
#include "Animation.h"
class HealthPotion :
	public GameObject
{
public:
	HealthPotion();
	HealthPotion(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~HealthPotion();

	void AddComponent(Component* comp);
	void SetState(void(HealthPotion::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void(HealthPotion::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};