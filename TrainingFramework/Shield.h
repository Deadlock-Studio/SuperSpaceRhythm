#pragma once
#include "GameObject.h"
#include "Animation.h"
class Shield :
	public GameObject
{
public:
	Shield();
	Shield(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Shield();

	void AddComponent(Component* comp);
	void SetState(void(Shield::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Spawn();
	void Idle();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void Destroyed();
	void InitDestroyed();
	int status = 0;
	void(Shield::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int shieldDuration = 0;
};