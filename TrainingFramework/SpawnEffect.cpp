#include "stdafx.h"
#include "SpawnEffect.h"

SpawnEffect::SpawnEffect()
{
	SetState(&SpawnEffect::Idle);
}

SpawnEffect::SpawnEffect(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : SpawnEffect()
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

	SpawnEffectTick = 15;
	Init();
}


SpawnEffect::~SpawnEffect()
{
}


void SpawnEffect::Init()
{

}



void SpawnEffect::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void SpawnEffect::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void SpawnEffect::Idle()
{
	PlayAnimation(0);
	SpawnEffectTick--;
	if (SpawnEffectTick == 0) {
		SetState(&SpawnEffect::Destroyed);
	}
}

void SpawnEffect::Destroyed()
{
	SceneManager::GetInstance()->addToRemovalList(this);
}

void SpawnEffect::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	GameObject::Update(deltaTime);
}

void SpawnEffect::checkCollision(GameObject * tempObj)
{

}