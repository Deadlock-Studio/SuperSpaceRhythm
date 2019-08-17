#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobShoot :
	public GameObject
{
public:
	MobShoot();
	MobShoot(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobShoot();

	void AddComponent(Component* comp);
	void SetState(void(MobShoot::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	int status = 0;
	void(MobShoot::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};