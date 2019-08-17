#include "stdafx.h"
#include "Collision2D.h"


Collision2D::Collision2D()
{
}

Collision2D::Collision2D(float width, float height,float mass,float res, char * type)
{
	bodyW = width;
	bodyH = height;
	bodyMass = mass;
	bodyRes = res;
	bodyType = _strdup(type);
}

Component * Collision2D::Clone()
{
	Collision2D * col2D = new Collision2D(bodyW, bodyH, bodyMass, bodyRes, bodyType);
	return col2D;
}

void Collision2D::Draw()
{
}

void Collision2D::Update(float deltaTime)
{
}

void Collision2D::addToPosition(float x, float y)
{
	b2Vec2 position;
	position.x = x / PIXEL_RATIO;
	position.y = y / PIXEL_RATIO;
	body->SetLinearVelocity(position);
}

void Collision2D::UpdateBodyPostition(float x, float y)
{
	b2Vec2 bodyPos;
	bodyPos.x = x / PIXEL_RATIO;
	bodyPos.y = y / PIXEL_RATIO;
	body->SetTransform(bodyPos, 0);
}

void Collision2D::UpdateBodyScale(float x, float y)
{
	PhysicManager::GetInstance()->world->DestroyBody(body);
	bodyW = bodyW * x;
	bodyH = bodyH * y;
	Init();
}

void Collision2D::UpdateBodyRotation(float angle)
{
	b2Vec2 bodyPos;
	bodyPos.x = bodyX / PIXEL_RATIO;
	bodyPos.y = bodyY / PIXEL_RATIO;
	body->SetTransform(bodyPos, angle);
}

void Collision2D::Init()
{
	bodyX = ((GameObject*)parentObj)->transform->position.x;
	bodyY = ((GameObject*)parentObj)->transform->position.y;
	if (strcmp(((GameObject*)parentObj)->name, "pBullet_blue") == 0 || strcmp(((GameObject*)parentObj)->name, "pBullet_red") == 0 || strcmp(((GameObject*)parentObj)->name, "eBullet") == 0
		|| strcmp(((GameObject*)parentObj)->name, "shield") == 0) {
		isBullet = true;
	}
	else
		isBullet = false;
	body = PhysicManager::GetInstance()->createBody(bodyX, bodyY, bodyW, bodyH, bodyMass, bodyRes, bodyType, isBullet);
	body->SetUserData((GameObject*)parentObj);
}

Collision2D::~Collision2D()
{
	free(bodyType);
}
