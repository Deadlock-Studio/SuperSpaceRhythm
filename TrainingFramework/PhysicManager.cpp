#include "stdafx.h"
#include "PhysicManager.h"

PhysicManager* PhysicManager::ms_pInstance = NULL;


void PhysicManager::Init()
{
	b2Vec2 gravity(0.0f, 0.0f);
	world = new b2World(gravity);
	world->SetContactListener(&contactListener);
}

void PhysicManager::DeleteAll()
{
	delete world;
	world = NULL;
}

b2Body * PhysicManager::createBody(float x, float y, float width, float height,float mass,float res, char * type, bool isBullet)
{
	b2Body * tempBody;
	b2FixtureDef fd;
	b2BodyDef bd;
	b2CircleShape circleShape;
	b2PolygonShape shape;

	fd.density = mass;
	fd.restitution = res;

	if (isBullet) {
		circleShape.m_p.Set(0, 0);
		circleShape.m_radius = (width / 2) / PIXEL_RATIO;
		fd.shape = &circleShape;
	}
	else if (!isBullet) {
		shape.SetAsBox((width / 2) / PIXEL_RATIO, (height / 2) / PIXEL_RATIO);
		fd.shape = &shape;
	}
	bd.fixedRotation = true;
	if (!strcmp(type, "DYNAMIC")) {
		bd.type = b2_dynamicBody;
	}
	else if (!strcmp(type, "STATIC")) {
		bd.type = b2_staticBody;
	}
	else if (!strcmp(type, "KINEMATIC")) {
		bd.type = b2_kinematicBody;
	}
	bd.position.Set(x / PIXEL_RATIO, y / PIXEL_RATIO);
	
	tempBody = world->CreateBody(&bd);
	tempBody->CreateFixture(&fd);
	return tempBody;
}

void PhysicManager::Update(float deltaTime)
{
	world->Step(1.0f / FPS, Globals::velIter, Globals::posIter);
	SceneManager::GetInstance()->deleteRemovalList();
}
