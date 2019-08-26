#pragma once
#include "GameObject.h"
class Room :
	public GameObject
{
public:
	Room();
	Room(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Room();

	b2Body * bodyTop;
	b2Body * bodyBottom;
	b2Body * bodyLeft;
	b2Body * bodyRight;

	void checkCollision(GameObject * tempObj);
};

