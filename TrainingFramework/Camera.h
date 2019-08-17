#pragma once
#include "../Utilities/utilities.h"

class Camera
{
public:
	Camera();
	~Camera();
	void InitCamera();
	void UpdateCamera();
	void LookAt();
	void UpdatePosition(float x, float y, float deltaTime);

	float nearPlane;
	float farPlane;
	float fov;
	float speed;
	float rotSpeed;

	Vector3 position;
	Vector3 cameraDirection;
	Vector3 cameraRight;
	Vector3 cameraUp;

	Matrix m_direction;
	Matrix m_translation;
	Matrix m_perspective;
	Matrix m_view;
};

