#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobDino :
	public GameObject
{
public:
	MobDino();
	MobDino(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobDino();

	void AddComponent(Component* comp);
	void SetState(void(MobDino::* state)()) {
		activeState = state;
	}

	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Death();
	void Update(float deltaTime) override;
	void checkCollision(GameObject* tempObj);
	void Init();
	int status = 0;
	void(MobDino::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};