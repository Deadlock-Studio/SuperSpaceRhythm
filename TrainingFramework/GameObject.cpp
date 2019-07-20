#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "SceneManager.h"

GameObject::GameObject()
{
	AddComponent(new Transform());
}


GameObject::~GameObject()
{
}

void GameObject::updateTransform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	//Todo replace with get component
	if (!componentList.empty())
		for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
			if ((*it)->type == TRANSFORM) {
				Transform * trans = (Transform*)(*it);
				trans->updateTransform(position, rotation, scale);
			}
		}
}

void GameObject::AddComponent(Component * comp)
{
	for (std::vector<ComponentType>::iterator it = typeList.begin(); it != typeList.end(); ++it) {
		if ((*it) == comp->type) {
			cout << "An object can only have one of each type of component\n";
			return;
		}
	}

	componentList.push_back(comp);
	typeList.push_back(comp->type);
	comp->parentObj = this;
}

void GameObject::Print()
{
	for (std::vector<ComponentType>::iterator it = typeList.begin(); it != typeList.end(); ++it) {
		cout << *it << "\n";
	}
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