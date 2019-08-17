#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobWiz :
	public GameObject
{
public:
	MobWiz();
	MobWiz(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobWiz();

	void AddComponent(Component* comp);
	void SetState(void(MobWiz::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	void CalculateVelocity(float mX, float mY);
	float mX, mY, x, y, velX, velY, distance;
	int status = 0;
	void(MobWiz::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};