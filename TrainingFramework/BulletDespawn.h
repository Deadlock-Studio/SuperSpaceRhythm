#pragma once
#include "GameObject.h"
#include "Animation.h"
class BulletDespawn :
	public GameObject
{
public:
	BulletDespawn();
	BulletDespawn(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~BulletDespawn();

	void AddComponent(Component* comp);
	void SetState(void(BulletDespawn::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	int status = 0;
	void(BulletDespawn::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int BulletDespawnTick;
};