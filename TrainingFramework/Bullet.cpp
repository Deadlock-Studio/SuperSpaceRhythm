#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
	//SetState(&Bullet::Idle);
}

Bullet::Bullet(float x, float y, float mX, float mY, char* type)
{
	name = _strdup(type);
	Blueprint* blueprint = SceneManager::GetInstance()->GetBlueprintByName("bullet");
	for (vector<Component*>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}
	this->x = x;
	this->y = y;
	this->mX = mX;
	this->mY = mY;
	UpdatePosition(x, y, EFFECT_LAYER);
	Init(name);
}


Bullet::~Bullet()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}

//init bullet red attr
void Bullet::Init(char* type)
{
	CalculateVelocity();
	if (strcmp(type, "pBullet_red") == 0) {
		PlayAnimation(0);
		UpdateScale(2.0f, 2.0f, 2.0f);
		//set collision filtering
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = BULLET_RED;
		//collide with what
		filter.maskBits = BOSS | WALL | MOB | MOB_RED | CRATE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
		damage = 15;
	}
	else if (strcmp(type, "pBullet_blue") == 0) {
		PlayAnimation(1);
		UpdateScale(1.5f, 1.5f, 1.5f);

		//set collision filtering
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = BULLET_BLUE;
		//collide with what
		filter.maskBits = BOSS | WALL | MOB | MOB_BLUE | CRATE | BULLET_BLUE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
		damage = 5;
	}
	else if (strcmp(type, "eBullet") == 0) {
		PlayAnimation(2);
		UpdateScale(2.5f, 2.5f, 2.5f);
		//set collision filtering
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = BULLET_E;
		//collide with what
		filter.maskBits = WALL | PLAYER | CRATE ; //add collision
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}

}

void Bullet::Despawn()
{
	if (strcmp(name, "pBullet_red") == 0) {
		GameManager::GetInstance()->Spawn("bulletDespawn",
			SceneManager::GetInstance()->GetBlueprintByName("bullet_despawn"),
			Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
			Vector3(2, 2, 2),
			Vector3());
	}
	else if (strcmp(name, "pBullet_blue") == 0) {
		GameManager::GetInstance()->Spawn("bulletDespawn",
			SceneManager::GetInstance()->GetBlueprintByName("bullet_despawn"),
			Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
			Vector3(1.5, 1.5, 1.5),
			Vector3());
	}
	else if (strcmp(name, "eBullet") == 0) {
		GameManager::GetInstance()->Spawn("bulletDespawn",
			SceneManager::GetInstance()->GetBlueprintByName("bullet_despawn"),
			Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
			Vector3(2.5, 2.5, 2.5),
			Vector3());
	}
	SceneManager::GetInstance()->addToRemovalList(this);
}


void Bullet::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Bullet::CalculateVelocity()
{
	float distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	if (strcmp(name, "pBullet_blue") == 0) {
		speed = 2.0f * MOVE_SPEED;
	}
	if (strcmp(name, "pBullet_red") == 0) {
		speed = 1.5f * MOVE_SPEED;
	}
	if (strcmp(name, "eBullet") == 0) {
		speed = 0.5f * MOVE_SPEED;
	}
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void Bullet::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}


void Bullet::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	//bullet fly
	AddToPosition(velX, velY);

	GameObject::Update(deltaTime);

}

void Bullet::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "boss") == 0 && (strcmp(name, "pBullet_red") == 0) || strcmp(name, "pBullet_blue") == 0) {
		tempObj->GetComponent<HP>()->Damage(damage);
		SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "crate") == 0 && (strcmp(name, "pBullet_red") == 0 || strcmp(name, "pBullet_blue") == 0 || strcmp(name, "eBullet") == 0)) {
		SetState(&Bullet::Despawn);
		((Crate*)tempObj)->SetState(&Crate::Exploding);
	}
	if (strcmp(tempObj->name, "tnt") == 0 && (strcmp(name, "pBullet_red") == 0 || strcmp(name, "pBullet_blue") == 0 || strcmp(name, "eBullet") == 0)) {
		SetState(&Bullet::Despawn);
		((TNT*)tempObj)->SetState(&TNT::Exploding);
	}
	if (strcmp(tempObj->name, "player") == 0 && strcmp(name, "eBullet") == 0) {
		SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "bomb") == 0) {
		SetState(&Bullet::Despawn);
		((Bomb*)tempObj)->SetState(&Bomb::Exploding);
	}
	if (strcmp(tempObj->name, "shield") == 0) {
		SetState(&Bullet::Despawn);
	}
}