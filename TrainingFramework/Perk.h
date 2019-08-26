#pragma once
#include "Text.h"
class Perk :
	public GameObject
{
public:
	Perk(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	~Perk();

	void checkCollision(GameObject * tempObj);
	void Init();

	void AddComponent(Component* comp);

	void Update(float deltaTime) override;

	Text* nameText;
	Text* descText;
};

