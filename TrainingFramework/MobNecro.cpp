#include "stdafx.h"
#include "MobNecro.h"
MobNecro::MobNecro()
{
	SetState(&MobNecro::Spawn);
}

MobNecro::MobNecro(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : MobNecro()
{
	name = _strdup(blueprint->name);


	//Clone components
	for (vector<Component*>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

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
	filter.maskBits = PLAYER | BULLET_RED | BULLET_BLUE | WALL | EXPLOSION | BOSS | CRATE | MOB | MOB_RED | MOB_BLUE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
	srand(time(NULL));
	mX = -69 + (std::rand() % (1347 - (-69) + 1));
	mY = -101 + (std::rand() % (811 - (-101) + 1));
	CalculateVelocity(mX, mY);
}


void MobNecro::CalculateVelocity(float mX, float mY)
{
	x = transform->position.x;
	y = transform->position.y;
	distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f * MOVE_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
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
	GameManager::GetInstance()->Spawn("smoke",
		SceneManager::GetInstance()->GetBlueprintByName("smoke"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3());
	SceneManager::GetInstance()->addToRemovalList(this);
}

void MobNecro::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	if (abs(transform->position.x - mX) <= 10 && abs(transform->position.y - mY) <= 10)
	{
		srand(time(NULL));
		mX = -69 + (std::rand() % (1347 - (-69) + 1));
		mY = -101 + (std::rand() % (811 - (-101) + 1));
		CalculateVelocity(mX, mY);
	}
	else
		AddToPosition(velX, velY);
	GameObject::Update(deltaTime);
}

void MobNecro::checkCollision(GameObject* tempObj)
{
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Exploding);
	}
	if (strcmp(tempObj->name, "pBullet_red") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
	if (strcmp(tempObj->name, "pBullet_blue") == 0) {
		GetComponent<HP>()->Damage(((Bullet*)tempObj)->damage);
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
}

