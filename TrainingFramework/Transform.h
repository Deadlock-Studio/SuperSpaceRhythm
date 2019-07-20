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
	Matrix getModelMatrix() { return m_model; }
	Vector3 getPosition() { return position; }
	Vector3 getRotation() { return rotation; }
	Vector3 getScale() { return scale; }
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

private:
	Matrix m_model;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

