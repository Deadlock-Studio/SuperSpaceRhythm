#pragma once
#include "GameObject.h"
#include "Animation.h"
class Bomb :
	public GameObject
{
public:
	Bomb();
	Bomb(float x, float y, float mX, float mY);
	~Bomb();

	void CalculateVelocity();

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
	float x, y, mX, mY, distance = 0, velX = 0, velY = 0;
	void(Bomb::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int bombCoolDown = 0;
	int bombDelay = 0;
};