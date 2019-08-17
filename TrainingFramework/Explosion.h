#pragma once
#include "GameObject.h"
#include "Animation.h"
class Explosion :
	public GameObject
{
public:
	Explosion();
	Explosion(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Explosion();

	void AddComponent(Component* comp);
	void SetState(void(Explosion::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void InitDestroyed();
	int status = 0;
	void(Explosion::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int explosionTick;
};