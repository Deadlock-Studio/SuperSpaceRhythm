#pragma once
#include "Component.h"
#include <vector>
using namespace std;

class Blueprint
{
public:
	Blueprint();
	virtual ~Blueprint();

	/*
	* Component Methods
	*/
	template <typename CheckType>
	CheckType * GetComponent();
	virtual void AddComponent(Component * comp) { componentList.push_back(comp); }

	char * name;
	vector<Component *> componentList{};
};

template<typename CheckType>
inline CheckType * Blueprint::GetComponent()
{
	if (!componentList.empty())
		for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
			if (dynamic_cast<CheckType *>(*it) != NULL) {
				return (CheckType*)(*it);
			}
		}
	return NULL;
}
