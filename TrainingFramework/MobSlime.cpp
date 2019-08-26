#include "stdafx.h"
#include "MobSlime.h"
MobSlime::MobSlime()
{
	SetState(&MobSlime::Spawn);
}

MobSlime::MobSlime(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobSlime()
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


MobSlime::~MobSlime()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobSlime::Init()
{
	if (strcmp(name, "mob_white") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB;
		//collide with what
		filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE | SHIELD;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}
	if (strcmp(name, "mob_red") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB_RED;
		//collide with what
		filter.maskBits = PLAYER | BULLET_RED | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE | SHIELD;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}
	if (strcmp(name, "mob_blue") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB_BLUE;
		//collide with what
		filter.maskBits = PLAYER | BULLET_BLUE | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE | SHIELD;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}
}



void MobSlime::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobSlime::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobSlime::CalculateVelocity()
{
	float mX, mY, x, y;

	mX = GameManager::GetInstance()->player->transform->position.x;
	mY = GameManager::GetInstance()->player->transform->position.y;
	x = transform->position.x;
	y = transform->position.y;
	float distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f *  MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void MobSlime::Idle()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
}

void MobSlime::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobSlime::Idle);
}

void MobSlime::Death()
{
	if (strcmp(this->name, "mob_white") == 0)
	{
		int offset = -60;
		int rdm = rand() % 2;
		for (int i = 0; i < 2; i++)
		{
			offset = offset * -1;
			if (rdm == 0)
			{
				GameManager::GetInstance()->SpawnToRoom("mob",
					SceneManager::GetInstance()->GetBlueprintByName("mob_red"),
					Vector3(Globals::clamp_x(transform->position.x + offset), Globals::clamp_y(transform->position.y + offset), MOB_LAYER),
					Vector3(1, 1, 1),
					Vector3());
			}
			if (rdm == 1)
			{
				GameManager::GetInstance()->SpawnToRoom("mob",
					SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
					Vector3(Globals::clamp_x(transform->position.x + offset), Globals::clamp_y(transform->position.y + offset), MOB_LAYER),
					Vector3(1, 1, 1),
					Vector3());
			}
		}

		GameManager::GetInstance()->mobCount -= 2;
	}

	GameManager::GetInstance()->Spawn("smoke",
		SceneManager::GetInstance()->GetBlueprintByName("smoke"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3());
	GameManager::GetInstance()->addToRemovalList(this);
	GameManager::GetInstance()->mobCount++;
}

void MobSlime::Update(float deltaTime)
{
	//Transition
	if (GetComponent<HP>()->dead) {
		SetState(&MobSlime::Death);
		if (((Player*)(GameManager::GetInstance()->player))->SpeedBoost)
		{
			((Player*)(GameManager::GetInstance()->player))->speedIncrease = TRUE;
		}
	}

	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	CalculateVelocity();
	AddToPosition(velX, velY);

	GameObject::Update(deltaTime);
}

void MobSlime::checkCollision(GameObject * tempObj)
{

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
}
