#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
	type = TRANSFORM;
	m_model.SetIdentity();
}

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale) : Transform()
{
	updateTransform(position, rotation, scale);
}

Transform::~Transform()
{
}

void Transform::CalculateMatrix()
{
	Matrix _trans;
	Matrix _scale;
	Matrix _rotate[4];

	_trans.SetTranslation(position.x, position.y, position.z);
	_scale.SetScale(scale.x, scale.y, scale.z);
	_rotate[0].SetRotationX(rotation.x);
	_rotate[1].SetRotationY(rotation.y);
	_rotate[2].SetRotationZ(rotation.z);
	_rotate[3] = _rotate[0] * _rotate[1] * _rotate[2];

	m_model = _scale * _rotate[3] * _trans;
}

void Transform::updateTransform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	CalculateMatrix();
}

void Transform::setPosition(float x, float y, float z)
{
	position.x = x; position.y = y; position.z = z;
	CalculateMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
	rotation.x = x; rotation.y = y; rotation.z = z;
	CalculateMatrix();
}

void Transform::setScale(float x, float y, float z)
{
	scale.x = x; scale.y = y; scale.z = z;
	CalculateMatrix();
}
