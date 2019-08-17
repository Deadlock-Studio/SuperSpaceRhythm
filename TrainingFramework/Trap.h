#pragma once
#include "GameObject.h"
#include "Animation.h"
class Trap :
	public GameObject
{
public:
	Trap();
	Trap(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Trap();

	void AddComponent(Component* comp);
	void SetState(void(Trap::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Active();
	void Prepare();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void InitIdle();
	void InitActive();
	int status = 0;
	void(Trap::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	void TrapOperation();
	int trapCoolDown = 0;
	int trapActive = 0;
	int trapPrepare = 0;
};