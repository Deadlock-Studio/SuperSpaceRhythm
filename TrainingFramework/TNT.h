#pragma once
#include "GameObject.h"
#include "Animation.h"
class TNT :
	public GameObject
{
public:
	TNT();
	TNT(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~TNT();

	void AddComponent(Component* comp);
	void SetState(void(TNT::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Destroying();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void InitDestroyed();
	int status = 0;
	void(TNT::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int TNTCoolDown = 0;
};