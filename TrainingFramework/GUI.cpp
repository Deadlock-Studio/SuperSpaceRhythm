#include "stdafx.h"
#include "GUI.h"


GUI::GUI()
{
}

GUI::GUI(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : GameObject::GameObject(blueprint, pos, scale, rotation)
{
	screenLocation = transform->m_model;
	SnapToCam();
}


GUI::~GUI()
{
}

void GUI::Update(float deltaTime)
{
}

void GUI::LateUpdate(float deltaTime)
{
	SnapToCam();
}

void GUI::UpdateTransform(Vector3 position, Vector3 rotation, Vector3 scale)
{

}

void GUI::UpdatePosition(float x, float y, float z)
{
	GameObject::UpdatePosition(x, y, z);
	screenLocation = transform->m_model;
	SnapToCam();
}

void GUI::UpdateRotation(float x, float y, float z)
{
}

void GUI::UpdateScale(float x, float y, float z)
{
}

void GUI::SnapToCam()
{
	Camera* cam = SceneManager::GetInstance()->usedCamera;
	Matrix m_gui_trans;
	m_gui_trans.SetTranslation(cam->position.x, cam->position.y, transform->position.z);
	transform->m_model = screenLocation * m_gui_trans;
}
