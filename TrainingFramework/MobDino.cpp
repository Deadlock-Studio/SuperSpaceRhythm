#include "stdafx.h"
#include "MobDino.h"
MobDino::MobDino()
{
	SetState(&MobDino::Spawn);
}

MobDino::MobDino(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobDino()
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


MobDino::~MobDino()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobDino::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MOB;
	//collide with what
	filter.maskBits =  BULLET_RED | BULLET_BLUE | WALL | EXPLOSION  | CRATE | SHIELD;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	mX = (int)X_MIN + (std::rand() % ((int)X_MAX - (int)X_MIN + 1));
	mY = (int)Y_MIN + (std::rand() % ((int)Y_MAX - (int)Y_MIN + 1));
	CalculateVelocity(mX, mY);
}

void MobDino::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobDino::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobDino::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobDino::Idle);
}

void MobDino::CalculateVelocity(float mX, float mY)
{
	x = transform->position.x;
	y = transform->position.y;
	distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f * MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void MobDino::Idle()
{
	//Transition
	if (GetComponent<HP>()->dead) {
		if (((Player*)(GameManager::GetInstance()->player))->SpeedBoost)
		{
			((Player*)(GameManager::GetInstance()->player))->speedIncrease = TRUE;
		}
		SetState(&MobDino::Death);
		return;
	}

	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
}

void MobDino::Death()
{
	GameManager::GetInstance()->Spawn("smoke",
		SceneManager::GetInstance()->GetBlueprintByName("smoke"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3());
	GameManager::GetInstance()->addToRemovalList(this);
	GameManager::GetInstance()->mobCount++;
}

void MobDino::Update(float deltaTime)
{
	//Transition
	if (GetComponent<HP>()->dead) {
		SetState(&MobDino::Death);
	}

	if (activeState != NULL)
		(this->*activeState)();
	CalculateVelocity(mX, mY);

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	SetState(&MobDino::Idle);
	
	if (mineCoolDown != 0)
	{
		mineCoolDown--;
	}
	else if (mineCoolDown == 0)
	{
		mineCoolDown = 500;
		GameManager::GetInstance()->SpawnToRoom("mine",
			SceneManager::GetInstance()->GetBlueprintByName("mine"),
			Vector3(transform->position.x, transform->position.y, TRAP_LAYER),
			Vector3(1, 1, 1),
			Vector3());
	}

	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	if (abs(transform->position.x - mX) <= 15 && abs(transform->position.y - mY) <= 15)
	{
		mX = (int)X_MIN + (std::rand() % ((int)X_MAX - (int)X_MIN + 1));
		mY = (int)Y_MIN + (std::rand() % ((int)Y_MAX - (int)Y_MIN + 1));
	}
	else
		AddToPosition(velX, velY);
	GameObject::Update(deltaTime);
}

void MobDino::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "pBullet_red") == 0
		|| strcmp(tempObj->name, "pBullet_blue_crit") == 0
		|| strcmp(tempObj->name, "pBullet_red_crit") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		GetComponent<HP>()->Damage(((Explosion*)tempObj)->damage);
	}
}

