#pragma once
#include "GameObject.h"
class GUI :
	public GameObject
{
public:
	GUI();
	GUI(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~GUI();

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void UpdateTransform(Vector3 position, Vector3 rotation, Vector3 scale);
	void UpdatePosition(float x, float y, float z);
	void UpdateRotation(float x, float y, float z);
	void UpdateScale(float x, float y, float z);
	void SnapToCam();

	Matrix screenLocation;
};

