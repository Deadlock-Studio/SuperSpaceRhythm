#pragma once
#include "GameObject.h"
#include "Animation.h"
class MobNecro :
	public GameObject
{
public:
	MobNecro();
	MobNecro(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~MobNecro();

	void AddComponent(Component* comp);
	void SetState(void(MobNecro::* state)()) {
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
	void(MobNecro::* activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
};