#include "stdafx.h"
#include "HP.h"


HP::HP()
{
}

HP::HP(float health)
{
	this->health = health;
}


HP::~HP()
{
}

void HP::Update(float deltaTime)
{
}

Component * HP::Clone()
{
	HP* clone = new HP();
	clone->health = health;
	return clone;
}

void HP::Damage(float damage)
{
	health -= damage;

	if (health <= 0)
		dead = true;
}
