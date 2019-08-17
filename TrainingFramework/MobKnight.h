#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobKnight :
	public GameObject
{
public:
	MobKnight();
	MobKnight(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobKnight();

	void AddComponent(Component* comp);
	void SetState(void(MobKnight::*state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init();
	void ShieldUp();

	void CalculateVelocity();
	float velX, velY;
	int status = 0;
	void(MobKnight::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int shieldCooldown = 0;
	int shieldDuration = 0;
	float distance;
	bool isShieldUp = false;
};