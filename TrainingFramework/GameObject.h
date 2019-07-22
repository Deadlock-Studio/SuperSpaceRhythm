#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include <vector>
using namespace std;

class GameObject
{
public:
	GameObject();
	~GameObject();

	/*
	* System Methods
	*/
	void Update();
	void Draw();

	/*
	* Component Methods
	*/
	void updateTransform(Vector3 position, Vector3 rotation, Vector3 scale);
	void AddComponent(Component * comp);

	template <typename CheckType>
	CheckType * GetComponent();

	void setActive(bool state) { isActive = state; }
	
	//Debug
	void Print();

	bool isActive = true;
	Transform * transform = NULL;
	vector<Component *> componentList{};
};

template<typename CheckType>
CheckType * GameObject::GetComponent()
{
	if (!componentList.empty())
		for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
			if (dynamic_cast<CheckType *>(*it) != NULL) {
				return (CheckType*)(*it);
			}
		}
	return NULL;
}
