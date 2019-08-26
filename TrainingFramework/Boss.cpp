#include "stdafx.h"
#include"Boss.h"

int timer = 0;
int actionTime = 0;
bool state = FALSE;
int bombcount = 0;

Boss::Boss()
{
	SetState(&Boss::Spawn);
}


Boss::~Boss()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}

Boss::Boss(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Boss()
{
	name = _strdup(blueprint->name);

	//Clone components
	for (vector<Component *>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	Init();
}

void Boss::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = BOSS;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | WALL | EXPLOSION | CRATE | CRATE | MOB | MOB_RED | MOB_BLUE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Boss::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Boss::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Boss::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(3, 3, 3),
		Vector3());
	SetState(&Boss::Idle);
}
void Boss::Idle()
{
	//State execution
	PlayAnimation(0);
}

void Boss::WalkLeft()
{
	//State execution
	PlayAnimation(0);
}

void Boss::WalkRight()
{
	//State execution
	PlayAnimation(0);
}

void Boss::Death()
{
	GameManager::GetInstance()->addToRemovalList(this);
	GameManager::GetInstance()->mobCount++;
}

void Boss::CalculateVelocity(float mX, float mY)
{
	x = transform->position.x;
	y = transform->position.y;
	distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.2f * MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void Boss::DoStuff() {
	int hp = GetComponent<HP>()->health;
	//phase 1
	if (hp >= 750)
	{

		CalculateVelocity(GameManager::GetInstance()->player->transform->position.x, 
			GameManager::GetInstance()->player->transform->position.y);
		AddToPosition(velX, velY);
		PlayAnimation(0);

		if (actionTime <= 10)
		{
			if (SoundManager::GetInstance()->enemySignal == 1)
			{
				//SceneManager::GetInstance()->SpawnBomb(this->transform->position.x, this->transform->position.y,
				//	GameManager::GetInstance()->player->transform->position.x, GameManager::GetInstance()->player->transform->position.y);
				BulletManager::GetInstance()->Trace(transform->position, "eBullet");
				actionTime++;
			}
		}
		else if (actionTime > 10 && actionTime <= 30)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->ShotGun(transform->position, "eBullet");
				actionTime++;
			}

		}
		else if (actionTime > 30 && actionTime <= 100)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				if (actionTime % 5)
				{
					BulletManager::GetInstance()->Cross(transform->position, "eBullet");
				}
				else if (actionTime % 15)
				{
					BulletManager::GetInstance()->Plus(transform->position, "eBullet");
				}
				actionTime++;
			}
		}

		//reset
		if (actionTime == 100)
		{
			actionTime = 0;
		}
	}

	//phase 2
	else if (hp >= 500 && hp < 750)
	{
		if (actionTime > 50)
		{
			actionTime = 0;
		}
		CalculateVelocity(X_MAX / 2, Y_MAX /2);
		AddToPosition(velX, velY);

		PlayAnimation(1);
		if (actionTime <= 10)
		{
			if (SoundManager::GetInstance()->enemySignal == 2)
			{
				//SceneManager::GetInstance()->SpawnBomb(this->transform->position.x, this->transform->position.y,
				//	GameManager::GetInstance()->player->transform->position.x, GameManager::GetInstance()->player->transform->position.y);
				BulletManager::GetInstance()->SuperTrace(transform->position, "eBullet");
				actionTime++;
			}
		}
		else if (actionTime > 10 && actionTime <= 30)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->ClockwiseSpriral(transform->position, "eBullet");
				actionTime++;
			}
	/*		if (actionTime % 10 == 0)
			{
				GameManager::GetInstance()->SpawnToRoom("mob",
					SceneManager::GetInstance()->GetBlueprintByName("mob_white"),
					Vector3(Globals::clamp_x(transform->position.x - 200 + (rand() % (200 + 200 + 1))),
						Globals::clamp_y(transform->position.y + -200 + (rand() % (200 + 200 + 1))), MOB_LAYER),
					Vector3(1, 1, 1),
					Vector3());
				actionTime++;
			}*/
		}
		else if (actionTime > 30 && actionTime <= 50)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->CounterClockwiseSpiral(transform->position, "eBullet");
				actionTime++;
			}
		}
		//reset
		if (actionTime == 50)
		{
			actionTime = 0;
		}
	}

	//phase 3
	else if (hp >= 250 && hp < 500)
	{
		if (actionTime > 50)
		{
			actionTime = 0;
		}
		CalculateVelocity(X_MAX / 2, Y_MAX / 2);
		AddToPosition(velX, velY);
		PlayAnimation(2);

		if (actionTime <= 20)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->DoubleSpiral(transform->position, "eBullet");
				actionTime++;
			}
		}
		else if (actionTime > 20 && actionTime <= 50)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				if (actionTime % 2 == 0)
				{
					BulletManager::GetInstance()->Cross(transform->position, "eBullet");
				}
				if (actionTime % 3 == 0)
				{
					BulletManager::GetInstance()->Plus(transform->position, "eBullet");
				}

				if (actionTime % 10 == 0)
				{
					SceneManager::GetInstance()->SpawnBomb(this->transform->position.x, this->transform->position.y,
						GameManager::GetInstance()->player->transform->position.x,
						GameManager::GetInstance()->player->transform->position.y);
				}
				actionTime++;
			}
		}
		//reset
		if (actionTime == 50)
		{
			actionTime = 0;
		}
	}

	//phase 4
	else if (hp > 0 && hp < 250)
	{
		if (actionTime > 80)
		{
			actionTime = 0;
		}
		CalculateVelocity(X_MAX / 2, Y_MAX / 2);
		AddToPosition(velX, velY);
		if (actionTime <= 30)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->Spiral(transform->position, "eBullet");
				actionTime++;
			}
		}

		else if (actionTime > 30 && actionTime <= 60) {
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				BulletManager::GetInstance()->Flower(transform->position, "eBullet");
				actionTime++;
			}
		}

		else if (actionTime > 60 && actionTime <= 80)
		{
			if (SoundManager::GetInstance()->enemySignal == 2 || SoundManager::GetInstance()->enemySignal == 1)
			{
				if (SoundManager::GetInstance()->enemySignal == 2)
				{
					BulletManager::GetInstance()->Circle(transform->position, "eBullet");
				}
				actionTime++;
			}
		}
		PlayAnimation(2);
		//reset
		if (actionTime == 80)
		{
			actionTime = 0;
		}
	}
}

void Boss::Update(float deltaTime)
{
	//Transition
	if (GetComponent<HP>()->dead) {
		SetState(&Boss::Death);
	}
	else SetState(&Boss::DoStuff);

	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	//AddToPosition(50.0f, 50.0f);
	//cout << GetComponent<HP>()->health << " " << actionTime << endl;
	GameObject::Update(deltaTime);
}

void Boss::checkCollision(GameObject * tempObj)
{
	//collide with player
	if (strcmp(tempObj->name, "player") == 0) {
		b2Vec2 playerPos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
		//if player touch boss
		if (playerPos.x != 0 || playerPos.y != 0) {
			tempObj->GetComponent<Control>()->Collide(playerPos.x,playerPos.y,TRUE,"boss");
		}
		//if boss touch player
		else {
			b2Vec2 bossPos = GetComponent<Collision2D>()->body->GetLinearVelocity();
			tempObj->GetComponent<Control>()->Collide(bossPos.x, bossPos.y,FALSE,"boss");
		}
	}
	//collide with player bullet
	if (strcmp(tempObj->name, "pBullet_red") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0
		|| strcmp(tempObj->name, "pBullet_blue_crit") == 0
		|| strcmp(tempObj->name, "pBullet_red_crit") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		GetComponent<HP>()->Damage(((Explosion*)tempObj)->damage);
	}
	if (strcmp(tempObj->name, "crate") == 0) {
		((Crate*)tempObj)->SetState(&Crate::Exploding);
	}
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Exploding);
	}
}


