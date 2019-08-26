#pragma once
#include "Text.h"
class PowerUp :
	public Component
{
public:
	PowerUp(char * name, char* desc);
	void ActivatePower();
	~PowerUp();

	Component * Clone() override;

	char* name;
	char* description;
};

