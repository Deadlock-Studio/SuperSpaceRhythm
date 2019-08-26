#include "stdafx.h"
#include "MobKnight.h"
MobKnight::MobKnight()
{
	SetState(&MobKnight::Spawn);
}

MobKnight::MobKnight(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobKnight()
{
	name = _strdup(blueprint->name);

	//Clone components
	for (vector<Component*>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	Init();
}


MobKnight::~MobKnight()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobKnight::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MOB;
	//collide with what
	filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | MOB | MOB_RED | MOB_BLUE | CRATE | SHIELD;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void MobKnight::ShieldUp()
{
	if (!isShieldUp) {
		shield = (GameManager::GetInstance()->SpawnToRoom("shield",
			SceneManager::GetInstance()->GetBlueprintByName("shield"),
			Vector3(transform->position.x, transform->position.y, MOB_LAYER),
			Vector3(1, 1, 1),
			Vector3()));
		GetComponent<Collision2D>()->body->GetFixtureList()->SetSensor(TRUE);
		isShieldUp = true;
	}
}

void MobKnight::ShieldDown() {
	isShieldUp = FALSE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetSensor(FALSE);
	((Shield*)shield)->SetState(&Shield::Destroyed);
}

void MobKnight::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobKnight::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobKnight::CalculateVelocity()
{
	float mX, mY, x, y;

	mX = GameManager::GetInstance()->player->transform->position.x;
	mY = GameManager::GetInstance()->player->transform->position.y;
	x = transform->position.x;
	y = transform->position.y;
	distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.6f * MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void MobKnight::Idle()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
}

void MobKnight::Delay()
{
	if (delay <= 50)
	{
		AddToPosition(0.0f, 0.0f);
		delay++;
	}
	else delay = 0;
}

void MobKnight::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobKnight::Idle);
}

void MobKnight::Death()
{
	if (isShieldUp)
		((Shield*)shield)->SetState(&Shield::Destroyed);

	GameManager::GetInstance()->Spawn("smoke",
		SceneManager::GetInstance()->GetBlueprintByName("smoke"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3());
	GameManager::GetInstance()->addToRemovalList(this);
	GameManager::GetInstance()->mobCount++;
}

void MobKnight::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	//Transition
	if (GetComponent<HP>()->dead) {
		if (((Player*)(GameManager::GetInstance()->player))->SpeedBoost)
		{
			((Player*)(GameManager::GetInstance()->player))->speedIncrease = TRUE;
		}
		SetState(&MobKnight::Death);
	}
	else if (delay > 0)
	{
		SetState(&MobKnight::Delay);
	}
	else
	{
		SetState(&MobKnight::Idle);
		if (shieldCooldown <= 0 && shieldDuration <= 200) {
			AddToPosition(0.0f, 0.0f);
			SetState(&MobKnight::ShieldUp);
			shieldDuration++;
			if (shieldDuration == 200)
				shieldCooldown = 200;
		}
		else if (shieldDuration >= 0) {
			if (isShieldUp)
			{
				shieldDuration = 0;
				SetState(&MobKnight::ShieldDown);
			}
			shieldCooldown--;
			AddToPosition(velX, velY);
		}
	}

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);

	CalculateVelocity();

	GameObject::Update(deltaTime);
}


void MobKnight::checkCollision(GameObject* tempObj)
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
	if (strcmp(tempObj->name, "player") == 0) {
		SetState(&MobKnight::Delay);
	}
}
