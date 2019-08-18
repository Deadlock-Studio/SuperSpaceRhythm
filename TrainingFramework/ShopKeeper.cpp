#include "stdafx.h"
#include "ShopKeeper.h"
#include "Control.h"


ShopKeeper::ShopKeeper()
{
	SetState(&ShopKeeper::Spawn);
}

ShopKeeper::ShopKeeper(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : ShopKeeper()
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
}


ShopKeeper::~ShopKeeper()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void ShopKeeper::Init()
{
	shopTick = 500;

	//filter MUST ALWAYS be the LAST THING to do in init
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = PLAYER;
	//collide with what
	filter.maskBits = PLAYER | WALL;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void ShopKeeper::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void ShopKeeper::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void ShopKeeper::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(2, 2, 2),
		Vector3());
	SetState(&ShopKeeper::Idle);
}

void ShopKeeper::Idle()
{
	PlayAnimation(0);
	shopTick--;
	if (shopTick <= 0) {
		SetState(&ShopKeeper::Death);
	}
}


void ShopKeeper::Death()
{
	GameManager::GetInstance()->Spawn("smoke",
		SceneManager::GetInstance()->GetBlueprintByName("smoke"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3());
	SceneManager::GetInstance()->addToRemovalList(this);
}



void ShopKeeper::checkCollision(GameObject* tempObj)
{
}


void ShopKeeper::Update(float deltaTime)
{
	b2Vec2 vec2 = GetComponent<Collision2D>()->body->GetPosition();
	if (activeState != NULL)
		(this->*activeState)();
	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	GameObject::Update(deltaTime);
}

