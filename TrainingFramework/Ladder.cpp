#include "stdafx.h"
#include "Ladder.h"

Ladder::Ladder()
{
	SetState(&Ladder::Spawn);
}

Ladder::Ladder(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Ladder()
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


Ladder::~Ladder()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Ladder::Init()
{
	GetComponent<Collision2D>()->body->SetSleepingAllowed(false);

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MINE;
	//collide with what
	filter.maskBits = PLAYER;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	GetComponent<Collision2D>()->body->GetFixtureList()->SetSensor(true);
}

void Ladder::SetCollidable(bool state)
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	filter.categoryBits = MINE;
	if (state)
		filter.maskBits = PLAYER;
	else filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}


void Ladder::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Ladder::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Ladder::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.5, 1.5, 1.5),
		Vector3());
	SetState(&Ladder::Idle);
}

void Ladder::Idle()
{
	PlayAnimation(0);
}

void Ladder::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();
	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	GameObject::Update(deltaTime);
}

void Ladder::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "player") == 0) {
		GameManager::GetInstance()->counter = 0;
		((Player*)tempObj)->SetState(&Player::ClimbLadder);
	}
}


