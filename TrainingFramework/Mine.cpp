#include "stdafx.h"
#include "Mine.h"

Mine::Mine()
{
	SetState(&Mine::Spawn);
}

Mine::Mine(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Mine()
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
	Init();
	MineCoolDown = 100;
}


Mine::~Mine()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Mine::Init()
{
	GetComponent<Collision2D>()->body->SetSleepingAllowed(false);
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MINE;
	//collide with what
	filter.maskBits = PLAYER | EXPLOSION | MOB | MINE | CRATE | WALL | ITEM;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	GetComponent<Collision2D>()->body->GetFixtureList()->SetSensor(true);

}

void Mine::InitDestroyed()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MINE;
	//collide with what
	filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}


void Mine::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Mine::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Mine::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(1.5, 1.5, 1.5),
		Vector3());
	SetState(&Mine::Idle);
}

void Mine::Idle()
{
	PlayAnimation(0);
}

void Mine::Destroying()
{
	PlayAnimation(1);
	InitDestroyed();
	MineCoolDown--;
	if (MineCoolDown == 0) {
		//spawn hp if Mine destroyed
		GameManager::GetInstance()->Spawn("explosion",
			SceneManager::GetInstance()->GetBlueprintByName("explosion"),
			Vector3(transform->position.x, transform->position.y, PLAYER_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		SetState(&Mine::Destroyed);

	}
}

void Mine::Destroyed()
{
	GameManager::GetInstance()->addToRemovalList(this);
}


void Mine::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	AddToPosition(0.0f, 0.0f);

	GameObject::Update(deltaTime);
}

void Mine::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&Mine::Destroying);
	}
}


