#pragma once
class HP :
	public Component
{
public:
	HP();
	HP(float health);
	~HP();

	void Update(float deltaTime) override;
	Component * Clone() override;

	void Damage(float damage);

	float health;
	bool dead = false;
};

