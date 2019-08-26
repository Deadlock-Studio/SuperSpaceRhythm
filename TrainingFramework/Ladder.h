#pragma once
#include "GameObject.h"
#include "Animation.h"
class Ladder :
	public GameObject
{
public:
	Ladder();
	Ladder(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Ladder();

	void AddComponent(Component* comp);
	void SetState(void(Ladder::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();

	void SetCollidable(bool state);

	int status = 0;
	void(Ladder::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};