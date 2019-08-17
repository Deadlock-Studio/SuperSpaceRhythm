#include "stdafx.h"
#include "TNT.h"

TNT::TNT()
{
	SetState(&TNT::Spawn);
}

TNT::TNT(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : TNT()
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
	Init();
	TNTCoolDown = 20;
}


TNT::~TNT()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void TNT::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = TNT_BOX;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | BOSS | EXPLOSION | MOB | TNT_BOX | CRATE | MOB | MOB_RED | MOB_BLUE | BULLET_E | WALL;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void TNT::InitDestroyed()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = TNT_BOX;
	//collide with what
	filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}


void TNT::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void TNT::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void TNT::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(1.5, 1.5, 1.5),
		Vector3());
	SetState(&TNT::Idle);
}

void TNT::Idle()
{
	PlayAnimation(0);
}

void TNT::Destroying()
{
	PlayAnimation(1);
	InitDestroyed();
	TNTCoolDown--;
	if (TNTCoolDown == 0) {
		SetState(&TNT::Destroyed);
		//spawn hp if TNT destroyed
		GameManager::GetInstance()->Spawn("explosion",
			SceneManager::GetInstance()->GetBlueprintByName("explosion"),
			Vector3(transform->position.x, transform->position.y, PLAYER_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		TNTCoolDown = 10;
	}
}

void TNT::Destroyed()
{
	PlayAnimation(2);
	TNTCoolDown--;
	if (TNTCoolDown == 0) {
		SceneManager::GetInstance()->addToRemovalList(this);
	}
}


void TNT::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, 1);
	AddToPosition(0.0f, 0.0f);

	GameObject::Update(deltaTime);
}

void TNT::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "pBullet_red") == 0 || strcmp(tempObj->name, "pBullet_blue") == 0 || strcmp(tempObj->name, "eBullet") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
		SetState(&TNT::Destroying);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&TNT::Destroying);
	}
	if (strcmp(tempObj->name, "boss") == 0) {
		SetState(&TNT::Destroying);
	}
}


