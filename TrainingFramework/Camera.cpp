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
	m_perspective.SetOrtho((GLfloat)Globals::screenWidth, 0, 0, (GLfloat)Globals::screenHeight, -10.0f, 10.0f);

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

	m_direction.m[0][0] = cameraRight.x; m_direction.m[0][1] = cameraUp.x; m_direction.m[0][2] = -cameraDirection.x; m_direction.m[0][3] = 0.0f;
	m_direction.m[1][0] = cameraRight.y; m_direction.m[1][1] = cameraUp.y; m_direction.m[1][2] = -cameraDirection.y; m_direction.m[1][3] = 0.0f;
	m_direction.m[2][0] = cameraRight.z; m_direction.m[2][1] = cameraUp.z; m_direction.m[2][2] = -cameraDirection.z; m_direction.m[2][3] = 0.0f;
	m_direction.m[3][0] = 0.0f; m_direction.m[3][1] = 0.0f; m_direction.m[3][2] = 0.0f; m_direction.m[3][3] = 1.0f;

	m_translation.SetTranslation(-position.x, -position.y, -position.z);

	m_view = m_translation * m_direction;
}

void Camera::UpdatePosition(float x, float y, float deltaTime)
{

	//secret gia truyền camera mechanic 
	position.x += (x - position.x)  * 0.05f;
	position.y += (y - position.y)  *  0.05f;
	m_translation.SetTranslation(-position.x, -position.y, -position.z);

	m_view = m_translation * m_direction;
}
