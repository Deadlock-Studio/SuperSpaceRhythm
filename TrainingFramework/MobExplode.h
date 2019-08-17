#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobExplode :
	public GameObject
{
public:
	MobExplode();
	MobExplode(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobExplode();

	void AddComponent(Component* comp);
	void SetState(void(MobExplode::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Explode();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void CalculateVelocity();
	void Init();
	int status = 0;
	void(MobExplode::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	float velX;
	float velY;
	float distance;
	float mX, mY, x, y;
	bool isExploding = false;
	int explodeDelay = 0;
};