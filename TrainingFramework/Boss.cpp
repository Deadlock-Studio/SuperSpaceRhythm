#include "stdafx.h"
#include"Boss.h"

int timer = 0;

Boss::Boss()
{
	SetState(&Boss::Spawn);
}


Boss::~Boss()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}

Boss::Boss(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Boss()
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

void Boss::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = BOSS;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | WALL | EXPLOSION | CRATE | CRATE | MOB | MOB_RED | MOB_BLUE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Boss::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Boss::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Boss::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(3, 3, 3),
		Vector3());
	SetState(&Boss::Idle);
}
void Boss::Idle()
{
	//State execution
	PlayAnimation(0);
}

void Boss::WalkLeft()
{
	//State execution
	PlayAnimation(0);
}

void Boss::WalkRight()
{
	//State execution
	PlayAnimation(0);
}

void Boss::Death()
{
}

void Boss::Update(float deltaTime)
{
	//if (timer < 100)
	//{
	//	timer++;
	//}
	//else if (timer == 100)
	//{
	//	timer = 0;
	//}

	/*if (SoundManager::GetInstance()->enemySignal == 1 || SoundManager::GetInstance()->enemySignal == 2)
	{
		BulletManager::GetInstance()->Triangle(transform->position);
	}*/

	if (SoundManager::GetInstance()->enemySignal == 2)
	{
		BulletManager::GetInstance()->Triangle(transform->position);
	}

	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	//AddToPosition(50.0f, 50.0f);
	
	GameObject::Update(deltaTime);
}

void Boss::checkCollision(GameObject * tempObj)
{
	//collide with player
	if (strcmp(tempObj->name, "player") == 0) {
		b2Vec2 playerPos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
		//if player touch boss
		if (playerPos.x != 0 || playerPos.y != 0) {
			tempObj->GetComponent<Control>()->Collide(playerPos.x,playerPos.y,TRUE,"boss");
		}
		//if boss touch player
		else {
			b2Vec2 bossPos = GetComponent<Collision2D>()->body->GetLinearVelocity();
			tempObj->GetComponent<Control>()->Collide(bossPos.x, bossPos.y,FALSE,"boss");
		}
	}
	//collide with player bullet
	if (strcmp(tempObj->name, "pBullet_red") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "crate") == 0) {
		((Crate*)tempObj)->SetState(&Crate::Exploding);
	}
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Exploding);
	}
}


