#pragma once
#include "Component.h"

class Transform
{
public:
	Transform();
	Transform(Vector3 position, Vector3 rotation, Vector3 scale);
	~Transform();
	
	/*
	* Methods
	*/
	void CalculateMatrix();
	void UpdateTransform(Vector3 position, Vector3 rotation, Vector3 scale);
	void UpdatePosition(Vector3 position);
	void UpdateRotation(Vector3 rotation);
	void UpdateScale(Vector3 scale);
	void addToPosition(float x, float y);

	/*
	* Getters/Setters
	*/
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	Matrix m_model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale = Vector3(1, 1, 1);

	void * parentObj = NULL;

};

