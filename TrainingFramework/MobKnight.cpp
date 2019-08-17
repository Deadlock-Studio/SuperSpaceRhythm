#include "stdafx.h"
#include "MobKnight.h"
MobKnight::MobKnight()
{
	SetState(&MobKnight::Spawn);
}

MobKnight::MobKnight(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobKnight()
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
	shieldCooldown = 100;
	shieldDuration = 100;
}


MobKnight::~MobKnight()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobKnight::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB;
		//collide with what
		filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | TNT_BOX | MOB | MOB_RED | MOB_BLUE | CRATE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void MobKnight::ShieldUp()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);

	shieldDuration--;
	AddToPosition(0.0f, 0.0f);
	if (!isShieldUp) {
		GameManager::GetInstance()->Spawn("shield",
			SceneManager::GetInstance()->GetBlueprintByName("shield"),
			Vector3(transform->position.x, transform->position.y, MOB_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		isShieldUp = true;
	}
	if (shieldDuration <= 0) {
		SetState(&MobKnight::Idle);
		shieldCooldown = 100;
	}

}

void MobKnight::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobKnight::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobKnight::CalculateVelocity()
{
	float mX, mY, x, y;

	mX = GameManager::GetInstance()->player->transform->position.x;
	mY = GameManager::GetInstance()->player->transform->position.y;
	x = transform->position.x;
	y = transform->position.y;
	distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f *  MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void MobKnight::Idle()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
	isShieldUp = false;

}

void MobKnight::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobKnight::Idle);
}
void MobKnight::Death()
{
}
void MobKnight::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x *PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);

	SetState(&MobKnight::ShieldUp);


	GameObject::Update(deltaTime);
}

void MobKnight::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Destroying);
	}
	if (strcmp(tempObj->name, "pBullet_red") == 0) {
		if (strcmp(name, "mob_red") == 0 || strcmp(name, "mob_white") == 0) {
			SceneManager::GetInstance()->addToRemovalList(tempObj);
			SceneManager::GetInstance()->addToRemovalList(this);
		}
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0) {
		if (strcmp(name, "mob_blue") == 0 || strcmp(name, "mob_white") == 0) {
			SceneManager::GetInstance()->addToRemovalList(tempObj);
			SceneManager::GetInstance()->addToRemovalList(this);
		}
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SceneManager::GetInstance()->addToRemovalList(this);
	}
}
