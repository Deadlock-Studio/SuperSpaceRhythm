#pragma once
#include"Box2D\Box2D.h"
#include "ContactListener.h"
#include "GameObject.h"

class PhysicManager
{
protected:
	PhysicManager() {}
	~PhysicManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	PhysicManager(const PhysicManager&) {}
	PhysicManager& operator =(const PhysicManager&) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new PhysicManager;
	}
	static PhysicManager* GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			GetInstance()->DeleteAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

protected:
	static PhysicManager* ms_pInstance;

public:
	b2World* world;
	ContactListener contactListener;
	b2Body* createBody(float x, float y, float width, float height,float mass,float res, char * type, bool isBullet);
	void Update(float deltaTime);
	void Init();
	void DeleteAll();
};