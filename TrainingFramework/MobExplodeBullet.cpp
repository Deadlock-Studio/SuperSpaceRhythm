#include "stdafx.h"
#include "MobExplodeBullet.h"
MobExplodeBullet::MobExplodeBullet()
{
	SetState(&MobExplodeBullet::Spawn);
}

MobExplodeBullet::MobExplodeBullet(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobExplodeBullet()
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
	explodeDelay = 50;

}


MobExplodeBullet::~MobExplodeBullet()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void MobExplodeBullet::Init()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = MOB;
	//collide with what
	filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | TNT_BOX | BOSS | MOB | MOB_RED | MOB_BLUE | CRATE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void MobExplodeBullet::CalculateVelocity()
{

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


void MobExplodeBullet::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void MobExplodeBullet::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void MobExplodeBullet::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(1, 1, 1),
		Vector3());
	SetState(&MobExplodeBullet::Idle);
}

void MobExplodeBullet::Idle()
{
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(1);
	else
		PlayAnimation(3);
}

void MobExplodeBullet::Death()
{
	AddToPosition(0.0f, 0.0f);
	isExploding = true;
	if (GameManager::GetInstance()->player->transform->position.x <= transform->position.x)
		PlayAnimation(0);
	else
		PlayAnimation(2);
	explodeDelay--;
	if (explodeDelay <= 0) {
		GameManager::GetInstance()->Spawn("explosion",
			SceneManager::GetInstance()->GetBlueprintByName("explosion"),
			Vector3(transform->position.x, transform->position.y, PLAYER_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		BulletManager::GetInstance()->Circle(this->transform->position);
		SceneManager::GetInstance()->addToRemovalList(this);
	}
}


void MobExplodeBullet::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, 1);
	CalculateVelocity();
	
	cout << distance << endl;
	if (distance <= 100.0f) {
		AddToPosition(0.0f, 0.0f);
		SetState(&MobExplodeBullet::Death);
		isExploding = true;
	}
	if(!isExploding)
		AddToPosition(velX, velY);
	GameObject::Update(deltaTime);
}

void MobExplodeBullet::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "pBullet_red") == 0 || strcmp(tempObj->name, "pBullet_blue") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
		SetState(&MobExplodeBullet::Death);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&MobExplodeBullet::Death);
	}
}

