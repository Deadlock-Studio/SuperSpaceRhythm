#include "stdafx.h"
#include "HealthPotion.h"


HealthPotion::HealthPotion()
{
	SetState(&HealthPotion::Idle);
}

HealthPotion::HealthPotion(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : HealthPotion()
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


HealthPotion::~HealthPotion()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void HealthPotion::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = ITEM;
	//collide with what
	filter.maskBits = PLAYER | EXPLOSION | WALL;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void HealthPotion::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void HealthPotion::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void HealthPotion::Idle()
{

	PlayAnimation(0);
}

void HealthPotion::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	

	AddToPosition(0.0f, 0.0f);

	GameObject::Update(deltaTime);
}

void HealthPotion::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "player") == 0) {
		GameManager::GetInstance()->addToRemovalList(this);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		GameManager::GetInstance()->addToRemovalList(this);
	}
}


