#include "stdafx.h"
#include "Explosion.h"

Explosion::Explosion()
{
	SetState(&Explosion::Idle);
}

Explosion::Explosion(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Explosion()
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
	
	explosionTick = 35;
	Init();
}


Explosion::~Explosion()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Explosion::Init()
{

	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = EXPLOSION;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | CRATE | BOSS | TNT_BOX | MOB | MOB_RED | MOB_BLUE | HEALTHPOTION | SHIELD;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);

}

void Explosion::InitDestroyed()
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = EXPLOSION;
	//collide with what
	filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}



void Explosion::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Explosion::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Explosion::Idle()
{
	PlayAnimation(0);
	explosionTick--;
	if (explosionTick == 0) {
		SetState(&Explosion::Destroyed);
	}
}

void Explosion::Destroyed()
{
	SceneManager::GetInstance()->addToRemovalList(this);
}

void Explosion::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)();

	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, 1);


	GameObject::Update(deltaTime);
}

void Explosion::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "crate") == 0) {
		((Crate*)tempObj)->SetState(&Crate::Destroying);
	}
	if (strcmp(tempObj->name, "tnt") == 0) {
		((TNT*)tempObj)->SetState(&TNT::Destroying);
	}
	if (strcmp(tempObj->name, "health_potion") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
	if (strcmp(tempObj->name, "mob_blue") == 0
		|| strcmp(tempObj->name, "mob_red") == 0
		|| strcmp(tempObj->name, "mob_white") == 0
		|| strcmp(tempObj->name, "mob_shoot") == 0
		|| strcmp(tempObj->name, "mob_explode") == 0){
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
}