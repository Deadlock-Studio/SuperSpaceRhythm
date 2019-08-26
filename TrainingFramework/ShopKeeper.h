#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Control.h"
class ShopKeeper :
	public GameObject
{
public:
	ShopKeeper();
	ShopKeeper(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~ShopKeeper();

	void AddComponent(Component* comp);
	void SetState(void(ShopKeeper::* state)()) {
		activeState = state;
	}

	void SetCollidable(bool state);

	void Update(float deltaTime);
	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void checkCollision(GameObject* tempObj);
	void Init();
	void(ShopKeeper::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};