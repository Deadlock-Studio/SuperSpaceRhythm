#pragma once
#include "Transform.h"
#include "Blueprint.h"
#include <vector>
using namespace std;

class GameObject : 
	public Blueprint
{
public:
	GameObject();
	GameObject(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	virtual ~GameObject();

	/*
	* System Methods
	*/
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Draw();

	/*
	* Component Methods
	*/
	void UpdateTransform(Vector3 position, Vector3 rotation, Vector3 scale);
	void UpdatePosition(float x, float y, float z);
	void UpdateRotation(float x, float y, float z);
	void UpdateScale(float x, float y, float z);
	void AddComponent(Component * comp);
	void AddToPosition(float x, float y);
	virtual void checkCollision(GameObject * tempObj);
	virtual void Init();
	bool isActive = true;
	Transform * transform = NULL;
};