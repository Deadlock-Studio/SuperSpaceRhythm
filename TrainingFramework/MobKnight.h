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
	void Stop();
	void checkCollision(GameObject * tempObj);
	void Init();
	void ShieldUp();

	void ShieldDown();

	void CalculateVelocity();
	float velX = 0.0f, velY = 0.0f;
	int status = 0;
	void(MobKnight::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int shieldCooldown = 200;
	int shieldDuration = 0;
	int delay = 0;
	GameObject* shield;
	float distance;
	bool isShieldUp = false;
};