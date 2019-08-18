#include "stdafx.h"
#include "BulletDespawn.h"

BulletDespawn::BulletDespawn()
{
	SetState(&BulletDespawn::Idle);
}

BulletDespawn::BulletDespawn(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : BulletDespawn()
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

	BulletDespawnTick = 10;
	Init();
}


BulletDespawn::~BulletDespawn()
{
}


void BulletDespawn::Init()
{

}



void BulletDespawn::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void BulletDespawn::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void BulletDespawn::Idle()
{
	PlayAnimation(0);
	BulletDespawnTick--;
	if (BulletDespawnTick == 0) {
		SetState(&BulletDespawn::Destroyed);
	}
}

void BulletDespawn::Destroyed()
{
	SceneManager::GetInstance()->addToRemovalList(this);
}

void BulletDespawn::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	GameObject::Update(deltaTime);
}

void BulletDespawn::checkCollision(GameObject* tempObj)
{

}