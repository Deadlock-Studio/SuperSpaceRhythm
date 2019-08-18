#pragma once
#include "GameObject.h"
#include "Animation.h"
class Smoke :
	public GameObject
{
public:
	Smoke();
	Smoke(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Smoke();

	void AddComponent(Component* comp);
	void SetState(void(Smoke::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	int status = 0;
	void(Smoke::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int SmokeTick;
};