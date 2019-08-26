#include "stdafx.h"
#include "Crate.h"

Crate::Crate()
{
	SetState(&Crate::Spawn);
}

Crate::Crate(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Crate()
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
	crateCoolDown = 20;
}


Crate::~Crate()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Crate::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = CRATE;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | BOSS | EXPLOSION | CRATE | BULLET_E | MOB | MOB_RED | MOB_BLUE | WALL | BOMB | TRAP;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void Crate::InitDestroyed()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = CRATE;
	//collide with what
	filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}


void Crate::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Crate::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Crate::Spawn()
{
	GameManager::GetInstance()->SpawnToRoom("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.5, 1.5, 1.5),
		Vector3());
	SetState(&Crate::Idle);
}

void Crate::Idle()
{
	PlayAnimation(0);
}

void Crate::Exploding()
{
	PlayAnimation(1);
	InitDestroyed();
	crateCoolDown--;
	if (crateCoolDown == 0) {
		SetState(&Crate::Destroyed);
		int a = std::rand() % 100;
		//spawn hp if crate destroyed
		if (a <= 25) {
			GameManager::GetInstance()->Spawn("hp",
				SceneManager::GetInstance()->GetBlueprintByName("health_potion"),
				Vector3(transform->position.x, transform->position.y, ITEM_LAYER),
				Vector3(1, 1, 1),
				Vector3());
		}
		crateCoolDown = 100;
	}
}

void Crate::Destroyed()
{
	PlayAnimation(2);
	crateCoolDown--;
	if (crateCoolDown == 0) {
		GameManager::GetInstance()->addToRemovalList(this);
	}
}


void Crate::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	AddToPosition(0.0f, 0.0f);


	GameObject::Update(deltaTime);
}

void Crate::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "pBullet_red") == 0 || strcmp(tempObj->name, "pBullet_blue") == 0 || strcmp(tempObj->name, "eBullet") == 0 || strcmp(tempObj->name, "eBullet_mob") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
		SetState(&Crate::Exploding);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&Crate::Exploding);
	}
}


