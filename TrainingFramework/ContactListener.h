#pragma once
#include "Box2D\Box2D.h"
#include "GameObject.h"
class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {
		GameObject* entityA = (GameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
		GameObject* entityB = (GameObject*)contact->GetFixtureB()->GetBody()->GetUserData();
		if (entityA && entityB)
			entityA->checkCollision(entityB);
	}

	void EndContact(b2Contact* contact) {

	}
};
