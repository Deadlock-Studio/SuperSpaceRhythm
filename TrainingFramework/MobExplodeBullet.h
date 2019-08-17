#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobExplodeBullet :
	public GameObject
{
public:
	MobExplodeBullet();
	MobExplodeBullet(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobExplodeBullet();

	void AddComponent(Component* comp);
	void SetState(void(MobExplodeBullet::*state)()) {
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
	void(MobExplodeBullet::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	float velX;
	float velY;
	float distance;
	float mX, mY, x, y;
	bool isExploding = false;
	int explodeDelay = 0;
};