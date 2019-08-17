#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobSlime :
	public GameObject
{
public:
	MobSlime();
	MobSlime(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobSlime();

	void AddComponent(Component* comp);
	void SetState(void(MobSlime::*state)()) {
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
	void(MobSlime::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};