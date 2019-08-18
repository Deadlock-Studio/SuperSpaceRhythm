#include "stdafx.h"
#include "Smoke.h"

Smoke::Smoke()
{
	SetState(&Smoke::Idle);
}

Smoke::Smoke(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Smoke()
{
	name = _strdup(blueprint->name);

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	//Clone components
	for (vector<Component*>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}

	SmokeTick = 35;
	Init();
}


Smoke::~Smoke()
{
}


void Smoke::Init()
{

}



void Smoke::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Smoke::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Smoke::Idle()
{
	PlayAnimation(0);
	SmokeTick--;
	if (SmokeTick == 0) {
		SetState(&Smoke::Destroyed);
	}
}

void Smoke::Destroyed()
{
	SceneManager::GetInstance()->addToRemovalList(this);
}

void Smoke::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	GameObject::Update(deltaTime);
}

void Smoke::checkCollision(GameObject* tempObj)
{

}