#pragma once
#include "GameObject.h"
#include "Animation.h"
class SpawnEffect :
	public GameObject
{
public:
	SpawnEffect();
	SpawnEffect(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~SpawnEffect();

	void AddComponent(Component* comp);
	void SetState(void(SpawnEffect::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	int status = 0;
	void(SpawnEffect::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int SpawnEffectTick;
};