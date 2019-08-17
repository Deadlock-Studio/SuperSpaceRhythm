#include "stdafx.h"
#include "Room.h"
#include "PhysicManager.h"

Room::Room()
{
	name = _strdup("room");

	//bottom wall
	body = PhysicManager::GetInstance()->createBody((float)Globals::screenWidth / 1.5f, -393.0f, 2000.0f, 10.0f, 1000.0f, 0.0f, "STATIC",false);
	body->SetUserData(this);
	//set collision filtering
	b2Filter filter = body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = WALL;
	//collide with what
	filter.maskBits = PLAYER | BULLET_BLUE | BULLET_RED | BOSS | MOB | MOB_BLUE | MOB_RED | BULLET_E | CRATE | ITEM | BOMB;
	body->GetFixtureList()->SetFilterData(filter);

	//top wall
	body = PhysicManager::GetInstance()->createBody((float)Globals::screenWidth / 2.0f, 1165.0f, 2000.0f, 10.0f, 1000.0f, 0.0f, "STATIC", false);
	body->SetUserData(this);
	body->GetFixtureList()->SetFilterData(filter);



	//left wall
	body = PhysicManager::GetInstance()->createBody(-112.0f, (float)Globals::screenHeight / 2.0f, 10.0f, 2000.0f, 1000.0f, 0.0f, "STATIC", false);
	body->SetUserData(this);
	body->GetFixtureList()->SetFilterData(filter);


	//right wall
	body = PhysicManager::GetInstance()->createBody(1391.0f, (float)Globals::screenHeight / 2.0f, 10.0f, 2000.0f, 1000.0f, 0.0f, "STATIC", false);
	body->SetUserData(this);
	body->GetFixtureList()->SetFilterData(filter);

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
}

void Room::checkCollision(GameObject * tempObj)
{
	//collide with player bullet
	if (strcmp(tempObj->name, "pBullet_red") == 0 || strcmp(tempObj->name, "pBullet_blue") == 0 || strcmp(tempObj->name, "eBullet") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
}
