#pragma once
#include "GameObject.h"
#include "Animation.h"
#include<math.h>
class Bullet :
	public GameObject
{
public:
	Bullet();
	Bullet(float x, float y, float mX, float mY, char * type);
	~Bullet();

	void AddComponent(Component* comp);
	void SetState(void(Bullet::*state)()) {
		activeState = state;
	}
	void CalculateVelocity();
	void PlayAnimation(int key);
	//void Idle();
	void Update(float deltaTime) override;
	void checkCollision(GameObject * tempObj);
	void Init(char * type);
	void Disable();

	void SetPosition(float x, float y, float mX, float mY);
	void(Bullet::*activeState)() = NULL;
	int activeAnimation = -1;
	float x, y, mX, mY, velX, velY;
	vector<Animation*> animeList{};

	float damage = 0;
};