#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	m_perspective.SetOrtho(Globals::screenWidth,0,0, Globals::screenHeight, 0.1, 500);

	position = Vector3(0.0, 0, 5);
	cameraDirection = Vector3(0.0, 0.0, -1.0);
	cameraUp = Vector3(0.0, 1.0, 0.0);

	LookAt();
}

void Camera::UpdateCamera()
{
	LookAt();
}

void Camera::LookAt()
{
	cameraDirection.Normalize();
	cameraUp.Normalize();
	cameraRight = cameraDirection.Cross(cameraUp);
	cameraRight.Normalize();
	cameraUp = cameraRight.Cross(cameraDirection);

	Matrix firstHalf;
	firstHalf.m[0][0] = cameraRight.x; firstHalf.m[0][1] = cameraUp.x; firstHalf.m[0][2] = -cameraDirection.x; firstHalf.m[0][3] = 0.0f;
	firstHalf.m[1][0] = cameraRight.y; firstHalf.m[1][1] = cameraUp.y; firstHalf.m[1][2] = -cameraDirection.y; firstHalf.m[1][3] = 0.0f;
	firstHalf.m[2][0] = cameraRight.z; firstHalf.m[2][1] = cameraUp.z; firstHalf.m[2][2] = -cameraDirection.z; firstHalf.m[2][3] = 0.0f;
	firstHalf.m[3][0] = 0.0f; firstHalf.m[3][1] = 0.0f; firstHalf.m[3][2] = 0.0f; firstHalf.m[3][3] = 1.0f;

	Matrix secondHalf;
	secondHalf.SetTranslation(-position.x, -position.y, -position.z);

	m_view = secondHalf * firstHalf;
}
