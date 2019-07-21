#pragma once
#include "Component.h"

class Transform :
	public Component
{
public:
	Transform();
	Transform(Vector3 position, Vector3 rotation, Vector3 scale);
	~Transform();
	
	/*
	* Methods
	*/
	void CalculateMatrix();
	void updateTransform(Vector3 position, Vector3 rotation, Vector3 scale);

	/*
	* Getters/Setters
	*/
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	Matrix m_model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

