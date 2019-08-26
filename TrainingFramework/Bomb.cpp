#include "stdafx.h"
#include "Bomb.h"
#define BOMB_COOLDOWN 250
#define BOMB_DELAY 50

Bomb::Bomb()
{
	
}



Bomb::Bomb(float x, float y, float mX, float mY)
{
	name = _strdup("bomb");
	Blueprint* blueprint = SceneManager::GetInstance()->GetBlueprintByName("bomb");
	for (vector<Component*>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}
	this->x = x;
	this->y = y;
	this->mX = mX;
	this->mY = mY;
	UpdatePosition(x, y, BULLET_LAYER);
	Init();
}

Bomb::~Bomb()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}

void Bomb::CalculateVelocity()
{
	float distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f * MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void Bomb::Init()
{
	CalculateVelocity();
	SetState(&Bomb::Idle);
	bombCoolDown = BOMB_COOLDOWN;
	bombDelay = BOMB_DELAY;

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = BOMB;
	//collide with what
	filter.maskBits = EXPLOSION | WALL | BULLET_RED | BULLET_BLUE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Bomb::AddComponent(Component* comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation*>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Bomb::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Bomb::Idle()
{
	PlayAnimation(1);
	bombCoolDown--;
	if (bombCoolDown <= 0) {
		SetState(&Bomb::Exploding);
	}
}

void Bomb::Destroyed()
{
	GameManager::GetInstance()->Spawn("explosion",
		SceneManager::GetInstance()->GetBlueprintByName("explosion"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	GameManager::GetInstance()->addToRemovalList(this);
}

void Bomb::Exploding()
{
	PlayAnimation(0);
	bombDelay--;
	if (bombDelay <= 0) {
		SetState(&Bomb::Destroyed);
	}
}

void Bomb::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	//bomb fly
	AddToPosition(velX, velY);

	GameObject::Update(deltaTime);
}

void Bomb::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "pBullet_red") == 0 || strcmp(tempObj->name, "pBullet_blue") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
		SetState(&Bomb::Exploding);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&Bomb::Exploding);
	}

}