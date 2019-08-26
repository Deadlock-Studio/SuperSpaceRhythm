#include "stdafx.h"
#include "Perk.h"
#include "PowerUp.h"
#include "Button.h"

Perk::Perk(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation)
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


Perk::~Perk()
{
	TextManager::GetInstance()->DeleteText(nameText);
	TextManager::GetInstance()->DeleteText(descText);

	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}

void Perk::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "player") == 0) {
		GetComponent<PowerUp>()->ActivatePower();
		GameManager::GetInstance()->spawnedBuffList.remove(this);
		GameManager::GetInstance()->shopped = true;
		GameManager::GetInstance()->addToRemovalList(this);
	}
}

void Perk::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = ITEM;
	//collide with what
	filter.maskBits = PLAYER | WALL;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

	nameText = TextManager::GetInstance()->
		AddText("monogram", GetComponent<PowerUp>()->name, Vector4(1.0, 1.0, 1.0, 1.0), transform->position.x - 30, transform->position.y + 80, 0.8, 0.8);
	nameText->isActive = false;
	descText = TextManager::GetInstance()->
		AddText("monogram", GetComponent<PowerUp>()->description, Vector4(1.0, 1.0, 1.0, 1.0), transform->position.x - 30, transform->position.y + 60, 0.6, 0.6);
	descText->isActive = false;
}

void Perk::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
}

void Perk::Update(float deltaTime)
{
	bool check = GetComponent<Button>()->isHovered;
	nameText->isActive = check;
	descText->isActive = check;

	GameObject::Update(deltaTime);
}

