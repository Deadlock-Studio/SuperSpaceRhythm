#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "SceneManager.h"

GameObject::GameObject()
{
	transform = new Transform();
}


GameObject::~GameObject()
{
}

void GameObject::updateTransform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	if (transform) {
		transform->updateTransform(position, rotation, scale);
	}
}

void GameObject::AddComponent(Component * comp)
{
	componentList.push_back(comp);
	comp->parentObj = this;
}

void GameObject::Print()
{
	
}

void GameObject::Update()
{
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		(*it)->Update();
	}
}

void GameObject::Draw()
{
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		(*it)->Draw();
	}
}