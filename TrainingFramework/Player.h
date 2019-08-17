#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Control.h"
class Player :
	public GameObject
{
public:
	Player();
	Player(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Player();

	void AddComponent(Component* comp);
	void SetState(void(Player::*state)()) {
		activeState = state;
	}
	void Update(float deltaTime);
	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Walk();
	void GetHit();
	void Dashing();
	void Death();
	void calculateAngle();
	void checkCollision(GameObject * tempObj);
	void Init();
	void(Player::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int iTick = 0;
	float angle;
};