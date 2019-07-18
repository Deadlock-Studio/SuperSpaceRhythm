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

	float nearPlane;
	float farPlane;
	float fov;
	float speed;
	float rotSpeed;

	Vector3 position;
	Vector3 cameraDirection;
	Vector3 cameraRight;
	Vector3 cameraUp;

	Matrix m_perspective;
	Matrix m_view;
};

