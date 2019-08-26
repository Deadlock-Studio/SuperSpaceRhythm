#include "stdafx.h"
#include "Control.h"
#include "Collision2D.h"
#include "Globals.h"
#include"Bullet.h"
#include "SoundManager.h"
#define DASH_TICK 20
#define COL_TICK 25

bool shoot = FALSE;
bool dash = FALSE;
bool reform = FALSE;
bool voidshell = FALSE;

Control::Control()
{
	inputMan = InputManager::GetInstance();
	dashSpeed = MOVE_SPEED / 20.0f;
	colSpeed = MOVE_SPEED / 50.0f;
}

Control::~Control()
{
}

void Control::Update(float deltaTime)
{
	//Movement
	//
	Vector2 moveOffset;
	Vector2 moveVector;
	Vector2 zero = Vector2();
	moveVector = (ActionCheck(MoveForward) ? Vector2(0, 1) : zero)
		+ (ActionCheck(MoveBackWard) ? Vector2(0, -1) : zero)
		+ (ActionCheck(MoveLeft) ? Vector2(-1, 0) : zero)
		+ (ActionCheck(MoveRight) ? Vector2(1, 0) : zero);
	if (!(moveVector == zero))
		moveVector.Normalize();
	if (((Player*)(GameManager::GetInstance()->player))->speedIncrease && spdBoostCounter < SPD_BOOST_TIME)
	{
		 moveOffset = moveVector * speed * 1.5;
		 spdBoostCounter++;
	}
	else if (spdBoostCounter >= SPD_BOOST_TIME)
	{
		((Player*)(GameManager::GetInstance()->player))->speedIncrease = FALSE;
		spdBoostCounter = 0;
	} 
	else moveOffset = moveVector * speed;

	//Dash
	//
	if (ActionCheck(Dash)) {
		if (!dash && !(moveVector == zero) && dashTick == 0 && (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3)) {
			dashTick = DASH_TICK;
			if (Globals::chance(((Player*)(GameManager::GetInstance()->player))->SideGunChance))
			{
				float x, y;
				x = ((GameObject*)parentObj)->transform->position.x;
				y = ((GameObject*)parentObj)->transform->position.y;

				float camPosX, camPosY;
				camPosX = SceneManager::GetInstance()->usedCamera->position.x;
				camPosY = SceneManager::GetInstance()->usedCamera->position.y;

				if (isReforming)
					SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_red");
				if (!isReforming)
					SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_blue");
			}
			dashVec = moveVector;
			dash = TRUE;
			((Player*)parentObj)->SetState(&Player::Dashing);
			
		}
	}
	else dash = FALSE;
	if (dashTick != 0) {
		dashTick--;
		isDashing = TRUE;
		dashVec.Normalize();
		((GameObject*)parentObj)->GetComponent<Collision2D>()->
			body->SetLinearVelocity(b2Vec2(dashVec.x * dashSpeed, dashVec.y * dashSpeed));
		//ur invulnerable code here
	}
	if (dashTick <= 0) {
		//regain control 
		isDashing = FALSE;
	}
	//Shoot
	//
	if (inputMan->isMouseDown) {
		if (!shoot)
		{
			float x, y;
			x = ((GameObject*)parentObj)->transform->position.x;
			y = ((GameObject*)parentObj)->transform->position.y;

			float camPosX, camPosY;
			camPosX = SceneManager::GetInstance()->usedCamera->position.x;
			camPosY = SceneManager::GetInstance()->usedCamera->position.y;
			if (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3)
			{
				if (isReforming) {
					if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->CritChance))
					{
						SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_red");
					}
					else SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_red_crit");
				}
				if (!isReforming) {
					if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->CritChance))
					{
						SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_blue");
					}
					else SceneManager::GetInstance()->SpawnBullet(x, y, inputMan->mX + camPosX, Globals::screenHeight - inputMan->mY + camPosY, "pBullet_blue_crit");
				}
			}
			shoot = TRUE;
		}
	}
	else shoot = FALSE;

	//Activate VoidShell
	//
	if (ActionCheck(VoidShell) && (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3))
	{
		vector<GameObject*>temp;
		if (!voidshell)
		{
			if (((Player*)(GameManager::GetInstance()->player))->voidShell > 0)
			{
				((Player*)(GameManager::GetInstance()->player))->voidShell--;
				for (std::list<GameObject*>::iterator it = GameManager::GetInstance()->roomObject.begin(); it != GameManager::GetInstance()->roomObject.end(); ++it) {
					if (strcmp((*it)->name, "eBullet") == 0 || strcmp((*it)->name, "eBullet_mob") == 0)
					{
						temp.push_back(*it);
					}
				}
				for (std::vector<GameObject*>::iterator it = temp.begin(); it != temp.end(); ++it) {
					((Bullet*)(*it))->SetState(&Bullet::Despawn);
				}
			}
		}
		voidshell = TRUE;
	}
	else voidshell = FALSE;
	//

	//Reform
	//
	if (inputMan->isRightMouseDown) {
		if (!reform && !disableControl && SoundManager::GetInstance()->signalPass == 4) {
			reform = TRUE;
			isReforming = !isReforming;
			if (strcmp(SoundManager::GetInstance()->currentlyPlaying->songname, "level") == 0)
			{
				if (!isReforming)
				{
					SoundManager::GetInstance()->currentlyPlaying->music.setVolume(SoundManager::GetInstance()->musicVolume);
					SoundManager::GetInstance()->getTrack("level_half")->music.setVolume(0);
				}
				else {
					SoundManager::GetInstance()->currentlyPlaying->music.setVolume(0);
					SoundManager::GetInstance()->getTrack("level_half")->music.setVolume(SoundManager::GetInstance()->musicVolume);
				}
			}
		}
	}
	else reform = FALSE;
	//execute logic if player touch boss
	//
	if (colTick > 0) {
		colTick--;

		//player being pushed back for amount of time
		((GameObject*)parentObj)->GetComponent<Collision2D>()->
			body->SetLinearVelocity(b2Vec2(-colVec.x * colSpeed, -colVec.y * colSpeed));
		//after pushed stun in place
		//regain control
		if (colTick <= 0) {
			colTick = 0;
			isColliding = FALSE;
		}
	}

	//if player are dashing or colliding then disable control
	if (isDashing || isColliding) {
		disableControl = TRUE;
	}
	else {
		disableControl = FALSE;
	}
	if (!disableControl) {
		((GameObject*)parentObj)->AddToPosition(moveOffset.x, moveOffset.y);
	}
}

Component * Control::Clone()
{
	return new Control();
}

bool Control::ActionCheck(Actions action)
{
	return inputMan->ActionCheck(action);
}
//signal if player touch boss
void Control::Collide(float x, float y, bool isPlayerTouching, char * type)
{
	isColliding = TRUE;
	//amount of stun time
	//colide with boss
	if (strcmp(type, "boss") == 0) {
		colTick = COL_TICK;
	}
	else colTick = COL_TICK;
	if (isPlayerTouching) {
		colVec.x = x;
		colVec.y = y;
	}
	else {
		colVec.x = -x;
		colVec.y = -y;
	}
	colVec.Normalize();
}
