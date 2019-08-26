#include "stdafx.h"
#include "Room.h"
#include "PhysicManager.h"

Room::Room()
{
	name = _strdup("room");

	//bottom wall
	bodyBottom = PhysicManager::GetInstance()->createBody((float)Globals::screenWidth / 1.5f, -240.0f, 2000.0f, 10.0f, 1000.0f, 0.0f, "STATIC",false);
	bodyBottom->SetUserData(this);
	//set collision filtering
	b2Filter filter = bodyBottom->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = WALL;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | BOSS | MOB | MOB_BLUE | MOB_RED | BULLET_E | CRATE | ITEM | BOMB;
	bodyBottom->GetFixtureList()->SetFilterData(filter);
	bodyBottom->SetSleepingAllowed(false);

	//top wall
	bodyTop = PhysicManager::GetInstance()->createBody((float)Globals::screenWidth / 2.0f, 1005.0f, 2000.0f, 10.0f, 1000.0f, 0.0f, "STATIC", false);
	bodyTop->SetUserData(this);
	bodyTop->GetFixtureList()->SetFilterData(filter);
	bodyTop->SetSleepingAllowed(false);



	//left wall
	bodyLeft = PhysicManager::GetInstance()->createBody(38.0f, (float)Globals::screenHeight / 2.0f, 10.0f, 2000.0f, 1000.0f, 0.0f, "STATIC", false);
	bodyLeft->SetUserData(this);
	bodyLeft->GetFixtureList()->SetFilterData(filter);
	bodyLeft->SetSleepingAllowed(false);


	//right wall
	bodyRight = PhysicManager::GetInstance()->createBody(1240.0f, (float)Globals::screenHeight / 2.0f, 10.0f, 2000.0f, 1000.0f, 0.0f, "STATIC", false);
	bodyRight->SetUserData(this);
	bodyRight->GetFixtureList()->SetFilterData(filter);
	bodyRight->SetSleepingAllowed(false);

}

Room::Room(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Room()
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
}

Room::~Room()
{
	bodyTop->GetWorld()->DestroyBody(bodyTop);
	bodyBottom->GetWorld()->DestroyBody(bodyBottom);
	bodyLeft->GetWorld()->DestroyBody(bodyLeft);
	bodyRight->GetWorld()->DestroyBody(bodyRight);

}

void Room::checkCollision(GameObject * tempObj)
{
	//collide with player bullet
	if (strcmp(tempObj->name, "pBullet_red") == 0 
		|| strcmp(tempObj->name, "pBullet_blue") == 0
		|| strcmp(tempObj->name, "pBullet_blue_crit") == 0
		|| strcmp(tempObj->name, "pBullet_red_crit") == 0
		|| strcmp(tempObj->name, "eBullet") == 0 
		|| strcmp(tempObj->name, "eBullet_mob") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
	}
}
