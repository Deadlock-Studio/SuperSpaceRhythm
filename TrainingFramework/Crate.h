#pragma once
#include "GameObject.h"
#include "Animation.h"
class Crate :
	public GameObject
{
public:
	Crate();
	Crate(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Crate();

	void AddComponent(Component* comp);
	void SetState(void(Crate::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Spawn();
	void Idle();
	void Exploding();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void InitDestroyed();
	int status = 0;
	void(Crate::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int crateCoolDown = 0;
};