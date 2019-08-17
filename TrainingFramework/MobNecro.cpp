#include "stdafx.h"
#include "MobNecro.h"
MobNecro::MobNecro()
{
	SetState(&MobNecro::Spawn);
}

MobNecro::MobNecro(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobNecro()
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


MobNecro::~MobNecro()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobNecro::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MOB;
	//collide with what
	filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | TNT_BOX | MOB | MOB_RED | MOB_BLUE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}




void MobNecro::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobNecro::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobNecro::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobNecro::Idle);
}

void MobNecro::Idle()
{
	//Transition
	if (GetComponent<HP>()->dead) {
		SetState(&MobNecro::Death);
		return;
	}

	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
}

void MobNecro::Death()
{
	SceneManager::GetInstance()->addToRemovalList(this);
}

void MobNecro::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	AddToPosition(0.0f, 0.0f);
	//SceneManager::GetInstance()->SpawnBullet(transform->position.x, transform->position.y, (GameManager::GetInstance())->player->transform->position.x, (GameManager::GetInstance())->player->transform->position.y, "pBullet_blue");
	GameObject::Update(deltaTime);
}

void MobNecro::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Destroying);
	}
	if (strcmp(tempObj->name, "pBullet_red") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
}

