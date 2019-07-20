#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "Component.h"
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
	
	//Debug
	void Print();

private:
	vector<Component *> componentList{};
	vector<ComponentType> typeList{};
	bool isActive = true;
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
