#include "stdafx.h"
#include "Trap.h"
#define TRAP_IDLE 0
#define TRAP_PREPARE 1
#define TRAP_ACTIVE 2

Trap::Trap()
{
	SetState(&Trap::Idle);
}

Trap::Trap(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Trap()
{
	name = _strdup(blueprint->name);

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	//Clone components
	for (vector<Component *>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}
	trapCoolDown = 120;
	InitIdle();
}


Trap::~Trap()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Trap::InitIdle()
{
	//trapCoolDown = 120;
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = TRAP;
	//collide with what
	filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Trap::InitActive()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = TRAP;
	//collide with what
	filter.maskBits = PLAYER;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Trap::TrapOperation()
{
	if (status == TRAP_IDLE) {
		trapCoolDown--;
		if (trapCoolDown == 0) {
			status = TRAP_PREPARE;
			SetState(&Trap::Prepare);
			trapPrepare = 40;
		}
	}
	else if (status == TRAP_PREPARE) {
		trapPrepare--;
		if (trapPrepare == 0) {
			status = TRAP_ACTIVE;
			SetState(&Trap::Active);
			trapActive = 40;
		}
	}
	else if (status == TRAP_ACTIVE) {
		trapActive--;
		if (trapActive == 0) {
			status = TRAP_IDLE;
			SetState(&Trap::Idle);
			trapCoolDown = 120;
		}
	}
}

void Trap::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Trap::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Trap::Idle()
{
	PlayAnimation(2);
	InitIdle();
}

void Trap::Active()
{
	PlayAnimation(0);
	InitActive();
}

void Trap::Prepare()
{
	PlayAnimation(1);
}



void Trap::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	TrapOperation();

	GameObject::Update(deltaTime);
}

void Trap::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "player") == 0) {
		//SceneManager::GetInstance()->addToRemovalList(this);
	}
}


