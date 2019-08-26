#include "stdafx.h"
#include "PowerUp.h"


PowerUp::PowerUp(char* name, char* desc)
{
	this->name = _strdup(name);
	this->description = _strdup(desc);
}

void PowerUp::ActivatePower() {
	//TODO Hard code
	if (strcmp(name, "Health") == 0)
	{
		GameManager::GetInstance()->player->GetComponent<HP>()->health = 10;
		GameManager::GetInstance()->perkList.at(0)->timeUsed++;
	}
	else if (strcmp(name, "Shield") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->shield += 3;
		GameManager::GetInstance()->perkList.at(2)->timeUsed++;
	}
	else if (strcmp(name, "Enhanced Muzzle") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->SpeedBulletChance = 5;
		GameManager::GetInstance()->perkList.at(5)->timeUsed++;
	}
	else if (strcmp(name, "Void Shell") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->voidShell += 1;
		GameManager::GetInstance()->perkList.at(1)->timeUsed++;
	}
	else if (strcmp(name, "Magic Beat") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->MagicBeatChance = 10;
		GameManager::GetInstance()->perkList.at(3)->timeUsed++;
	}
	else if (strcmp(name, "Side Gun") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->SideGunChance = 10;
		GameManager::GetInstance()->perkList.at(4)->timeUsed++;
	}
	else if (strcmp(name, "Blade Mail") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->ReflectChance = 40;
		GameManager::GetInstance()->perkList.at(6)->timeUsed++;
	}
	else if (strcmp(name, "Invincible Cloak") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->EvadeChance = 50;
		GameManager::GetInstance()->perkList.at(7)->timeUsed++;
	}
	else if (strcmp(name, "Swift Slipper") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->SpeedBoost = TRUE;
		GameManager::GetInstance()->perkList.at(8)->timeUsed++;
	}
	else if (strcmp(name, "Enhanced Shell") == 0)
	{
		((Player*)(GameManager::GetInstance()->player))->CritChance = 50;
		GameManager::GetInstance()->perkList.at(9)->timeUsed++;
	}
}

PowerUp::~PowerUp()
{
	free(name);
	free(description);
}

Component * PowerUp::Clone()
{
	PowerUp* pwr = new PowerUp(this->name, this->description);
	return pwr;
}                                                                                                                                                                                                                                                                                          