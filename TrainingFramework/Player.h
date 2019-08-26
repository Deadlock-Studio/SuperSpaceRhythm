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

	/*
	* State
	*/
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void PlayAnimation(int key);
	void Idle();
	void Spawn();
	void Walk();
	void GetHit();
	void Dashing();
	void Death();
	void ClimbLadder();

	/*
	* Technical
	*/
	void CalculateVelocity();
	void calculateAngle();
	void checkCollision(GameObject * tempObj);
	void Init();
	void InitDashing();
	void UpdateHP();
	void UpdateShield();
	void UpdateVoidShell();
	void(Player::*activeState)() = NULL;
	int activeAnimation = -1;
	vector<Animation*> animeList{};
	int iTick = 0;
	float angle;
	float velX, velY;
	bool immune = FALSE;
	bool speedIncrease = FALSE;

	void SetCollidable(bool state);

	/*
	* Fade
	*/
	void FadeOff();
	void FadeIn(float fadeSpeed);
	void FadeOut(float fadeSpeed);

	float fadeIn = 0;
	float fadeOut = 0;
	float alpha = 1;

	//powerups related
	int shield = 0;
	int voidShell = 0;
	int MagicBeatChance = 0;
	int SideGunChance = 0;
	int SpeedBulletChance = 0;
	int ReflectChance = 0;
	int EvadeChance = 0;
	bool SpeedBoost = FALSE;
	int CritChance = 0;
};