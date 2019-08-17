#include "stdafx.h"
#include "Mob1.h"
Mob1::Mob1()
{
	SetState(&Mob1::Spawn);
}

Mob1::Mob1(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Mob1()
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
}


Mob1::~Mob1()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Mob1::Init()
{
	if (strcmp(name, "mob_white") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB;
		//collide with what
		filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}
	if (strcmp(name, "mob_red") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB_RED;
		//collide with what
		filter.maskBits = PLAYER | BULLET_RED | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}
	if (strcmp(name, "mob_blue") == 0) {
		b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
		//type of body
		filter.categoryBits = MOB_BLUE;
		//collide with what
		filter.maskBits = PLAYER | BULLET_BLUE | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE | CRATE;
		GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	}


}



void Mob1::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Mob1::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Mob1::CalculateVelocity()
{
	float mX, mY, x, y;

	mX = GameManager::GetInstance()->player->transform->position.x;
	mY = GameManager::GetInstance()->player->transform->position.y;
	x = transform->position.x;
	y = transform->position.y;
	float distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f *  MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void Mob1::Idle()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(1);
}

void Mob1::Spawn()
{
		GameManager::GetInstance()->Spawn("spawn",
			SceneManager::GetInstance()->GetBlueprintByName("spawn"),
			Vector3(transform->position.x, transform->position.y, 2),
			Vector3(1, 1, 1),
			Vector3());
		SetState(&Mob1::Idle);
}void Mob1::Death()
{
}void Mob1::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x *PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	//CalculateVelocity();
	//AddToPosition(velX, velY);

	GameObject::Update(deltaTime);
}

void Mob1::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Exploding);
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
