#pragma once
#include "GameObject.h"
#include "Animation.h"
class Bomb :
	public GameObject
{
public:
	Bomb();
	Bomb(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Bomb();

	void AddComponent(Component* comp);
	void SetState(void(Bomb::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Destroyed();
	void Exploding();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	void(Bomb::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int bombCoolDown = 0;
	int bombDelay = 0;
};