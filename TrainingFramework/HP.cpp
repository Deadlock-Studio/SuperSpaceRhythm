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
	////health -= damage;
	//char damageText[50];
	////snprintf(damageText, sizeof(damageText), "%.0f!", damage);
	////TextManager::GetInstance()->AddText("COMIC",
	//	damageText, Vector4(0.85, 0.6, 0.5, 1.0),
	//	((Transform*)parentTrans)->position.x - 100,
	//	((Transform*)parentTrans)->position.y + 110,
	//	0.5, 0.5, 3);
}
