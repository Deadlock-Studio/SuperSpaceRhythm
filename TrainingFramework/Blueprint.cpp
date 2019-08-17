#include "stdafx.h"
#include "Blueprint.h"


Blueprint::Blueprint()
{
}


Blueprint::~Blueprint()
{
	free(name);
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		delete *it;
	}
	componentList.clear();
}
