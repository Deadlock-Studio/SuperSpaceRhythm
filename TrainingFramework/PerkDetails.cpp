#include "stdafx.h"
#include "PerkDetails.h"


PerkDetails::PerkDetails()
{
}

PerkDetails::PerkDetails(char * blueprint)
{
	this->blueprint = SceneManager::GetInstance()->GetBlueprintByName(blueprint);
}


PerkDetails::~PerkDetails()
{
}
