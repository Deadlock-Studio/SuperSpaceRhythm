#pragma once
#include "GameObject.h"
#include "Animation.h"
class Mine :
	public GameObject
{
public:
	Mine();
	Mine(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Mine();

	void AddComponent(Component* comp);
	void SetState(void(Mine::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Destroying();
	void Destroyed();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	void InitDestroyed();
	int status = 0;
	void(Mine::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int MineCoolDown = 0;
};