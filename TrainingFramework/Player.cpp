#include "stdafx.h"
#include "Player.h"
#include "Perk.h"
#include "Control.h"
#define iTICK 100

bool death = false; 

Player::Player()
{
	SetState(&Player::Spawn);

}

Player::Player(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Player()
{
	name = _strdup(blueprint->name);

	//Clone components
	for (vector<Component *>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}
	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	Init();
}


Player::~Player()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Player::Init()
{
	iTick = iTICK;
	immune = FALSE;
	//filter MUST ALWAYS be the LAST THING to do in init
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = PLAYER;
	//collide with what
	filter.maskBits = BOSS | ITEM| MOB | MOB_RED | MOB_BLUE | EXPLOSION | CRATE | CRATE | WALL | TRAP | BULLET_E | SHIELD | MINE | BOMB;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Player::InitDashing()
{
	immune = TRUE;
	//filter MUST ALWAYS be the LAST THING to do in init
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = PLAYER;
	//collide with what
	filter.maskBits =  ITEM  | WALL | SHIELD;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}

void Player::UpdateHP()
{
	float health = GetComponent<HP>()->health;
	int fullHealth = health / 2;
	int halfHealth = (int)health % 2;

	for (int i = 0; i < 5; i++) {
		if (i < fullHealth) {
			GameManager::GetInstance()->healthBar.at(i)->isActive = true;
			GameManager::GetInstance()->halfHealthBar.at(i)->isActive = false;
			GameManager::GetInstance()->emptyHealthBar.at(i)->isActive = false;
		}
		else if (halfHealth != 0) {
			GameManager::GetInstance()->healthBar.at(i)->isActive = false;
			GameManager::GetInstance()->halfHealthBar.at(i)->isActive = true;
			GameManager::GetInstance()->emptyHealthBar.at(i)->isActive = false;
			halfHealth = 0;
		}
		else {
			GameManager::GetInstance()->healthBar.at(i)->isActive = false;
			GameManager::GetInstance()->halfHealthBar.at(i)->isActive = false;
			GameManager::GetInstance()->emptyHealthBar.at(i)->isActive = true;
		}
	}


}

void Player::UpdateShield()
{
	for (int i = 0; i < 3; i++) {
		if (i < shield) {
			GameManager::GetInstance()->shieldBar.at(i)->isActive = true;
		}
		else GameManager::GetInstance()->shieldBar.at(i)->isActive = false;
	}
}

void Player::UpdateVoidShell()
{
	GameObject* shell = GameManager::GetInstance()->voidShell;
	if (voidShell > 0)
		shell->isActive = true;
	else shell->isActive = false;

	shell->UpdatePosition(transform->position.x, transform->position.y + 75, shell->transform->position.z);
}

void Player::SetCollidable(bool state)
{
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	filter.categoryBits = PLAYER;
	if (state)
		filter.maskBits = BOSS | ITEM | MOB | MOB_RED | MOB_BLUE | EXPLOSION | CRATE | CRATE | WALL | TRAP | BULLET_E | SHIELD | MINE | BOMB;
	else filter.maskBits = 0;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
}
void Player::FadeOff()
{
	fadeOut = 0;
	fadeIn = 0;
}

void Player::FadeIn(float fadeSpeed)
{
	fadeOut = 0;
	fadeIn = fadeSpeed;
}

void Player::FadeOut(float fadeSpeed)
{
	fadeIn = 0;
	fadeOut = fadeSpeed;
}

void Player::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Player::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Player::Spawn()
{
	GameManager::GetInstance()->Spawn("spawn",
		SceneManager::GetInstance()->GetBlueprintByName("spawn"),
		Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
		Vector3(2, 2, 2),
		Vector3());
	SetState(&Player::Idle);
}

void Player::Idle()
{
	Init();
	//Transition
	Control* control = GetComponent<Control>();
	if (control->ActionCheck(MoveLeft) || control->ActionCheck(MoveRight) || control->ActionCheck(MoveBackWard) || control->ActionCheck(MoveForward))
		SetState(&Player::Walk);
	else if (control->ActionCheck(Dash)) {
		SetState(&Player::Dashing);
	}
	////State execution
	if (GetComponent<Control>()->isReforming) {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(4); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(0); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(6); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(3);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(7);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(5); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(2); //8
		else PlayAnimation(1); //6
	}
	else {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(36);
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(32);
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(38);
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(35);
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(39);
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(37);
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(34);
		else PlayAnimation(33);
	}
}


void Player::Walk()
{
	//Transition
	Control* control = GetComponent<Control>();
	if (!control->ActionCheck(MoveRight) && !control->ActionCheck(MoveLeft) && !control->ActionCheck(MoveBackWard) && !control->ActionCheck(MoveForward))
		SetState(&Player::Idle);

	//State execution
	if (GetComponent<Control>()->isReforming) {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(12); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(8); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(14); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(11);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(15);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(13); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(10); //8
		else PlayAnimation(9); //6

	}
	else {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(44); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(40); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(46); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(43);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(47);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(45); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(42); //8
		else PlayAnimation(41); //6
	}
}

void Player::GetHit()
{
	if (Globals::chance(((Player*)(GameManager::GetInstance()->player))->ReflectChance) && !immune)
	{
		if (this->GetComponent<Control>()->isReforming)
			BulletManager::GetInstance()->Circle(transform->position, "pBullet_red");
		if (!this->GetComponent<Control>()->isReforming)
			BulletManager::GetInstance()->Circle(transform->position, "pBullet_blue");
	}
	immune = TRUE;
	if (GetComponent<Control>()->isReforming) {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(28); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(24); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(30); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(27);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(31);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(29); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(26); //8
		else PlayAnimation(25); //6

	}
	else {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(60); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(56); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(62); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(59);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(63);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(61); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(58); //8
		else PlayAnimation(57); //6
	}
	iTick--;
	if (iTick <= 0) {
		SetState(&Player::Idle);	
	}

}

void Player::Dashing()
{
	InitDashing();
	if (GetComponent<Control>()->isReforming) {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(20); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(16); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(22); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(19);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(23);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(21); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(18); //8
		else PlayAnimation(17); //6

	}
	else {
		if (angle <= 158.5 && angle > 112.5) PlayAnimation(52); //1
		else if (angle <= 202.5 && angle > 157.5) PlayAnimation(48); //2
		else if (angle <= 247.5 && angle > 202.5) PlayAnimation(54); //3
		else if (angle <= 292.5 && angle > 247.5) PlayAnimation(51);	//4
		else if (angle <= 337.5 && angle > 292.5) PlayAnimation(55);		//5
		else if (angle <= 67.5 && angle > 22.5) PlayAnimation(53); //7
		else if (angle <= 112.5 && angle > 67.5) PlayAnimation(50); //8
		else PlayAnimation(49); //6
	}
	if (GetComponent<Control>()->dashTick == 0) {
		SetState(&Player::Idle);
	}
}

void Player::Death()
{
	GetComponent<Control>()->isActive = false;
	GameManager::GetInstance()->HideHealthBar();
	if (!death) {
		GameManager::GetInstance()->Spawn("smoke",
			SceneManager::GetInstance()->GetBlueprintByName("smoke"),
			Vector3(transform->position.x, transform->position.y, EFFECT_LAYER),
			Vector3(2.0f, 2.0f, 2.0f),
			Vector3());
	}
	death = true;
	this->isActive = false;
}

void Player::ClimbLadder()
{
	GetComponent<Control>()->isActive = false;
	immune = true;
	SetCollidable(false);
	CalculateVelocity();
	AddToPosition(velX, velY);

	//When fade is done
	if (alpha == 0) {
		GetComponent<Control>()->isActive = true;
		SetState(&Player::Idle);
	}
}

void Player::CalculateVelocity()
{
	float mX, mY, x, y;

	mX = GameManager::GetInstance()->ladder->transform->position.x;
	mY = GameManager::GetInstance()->ladder->transform->position.y;
	x = transform->position.x;
	y = transform->position.y;
	float distance = sqrt(pow((mX - x), 2) + pow((mY - y), 2));
	float speed;
	speed = 0.5f *  PLAYER_WALK_SPEED;
	velX = ((mX - x) * speed / distance);
	velY = ((mY - y) * speed / distance);
}

void Player::calculateAngle()
{
	float mouseX, mouseY, x, y;
	mouseX = InputManager::GetInstance()->mouseX + SceneManager::GetInstance()->usedCamera->position.x;
	mouseY = InputManager::GetInstance()->mouseY + SceneManager::GetInstance()->usedCamera->position.y;
	x = transform->position.x;
	y = transform->position.y;
	angle = atan2(y - mouseY, x - mouseX);
	angle = float(angle * 180.0f / (float)PI + 180.0f);
}

void Player::checkCollision(GameObject * tempObj)
{
	if (strcmp(tempObj->name, "boss") == 0) {
		b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
		//if boss touch player
		if (pos.x != 0 || pos.y != 0) {
			b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, TRUE,"boss");
		}
		//if player touch boss
		else {
			b2Vec2 pos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE, "boss");
		}
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
	}
	if (strcmp(tempObj->name, "health_potion") == 0) {
		if (GameManager::GetInstance()->player->GetComponent<HP>()->health < 10)
		{
			GameManager::GetInstance()->player->GetComponent<HP>()->health += 1;
		}
		else if (GameManager::GetInstance()->player->GetComponent<HP>()->health < 10 &&
			GameManager::GetInstance()->player->GetComponent<HP>()->health >= 9)
		{
			GameManager::GetInstance()->player->GetComponent<HP>()->health = 10;
		}
		GameManager::GetInstance()->addToRemovalList(tempObj);
	}

	if (strcmp(tempObj->name, "trap") == 0) {
		b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
		if (pos.x != 0 || pos.y != 0) {
			GetComponent<Control>()->Collide(pos.x, pos.y, TRUE, "trap");
		}
		else {
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE,"trap");
		}
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
	}

	if (strcmp(tempObj->name, "crate") == 0) {
		if (GetComponent<Control>()->isDashing) {
			((Crate*)tempObj)->SetState(&Crate::Exploding);
		}
	}

	if (strcmp(tempObj->name, "mob_blue") == 0
		|| strcmp(tempObj->name, "mob_red") == 0 
		|| strcmp(tempObj->name, "mob_white") == 0) {
		((MobSlime*)tempObj)->SetState(&MobSlime::Death);
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
	}

	if (strcmp(tempObj->name, "explosion") == 0) {
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
	}

	if (strcmp(tempObj->name, "eBullet") == 0 || strcmp(tempObj->name, "eBullet_mob") == 0) {
		((Bullet*)tempObj)->SetState(&Bullet::Despawn);
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
	}

	if (strcmp(tempObj->name, "mob_knight") == 0) {
		((MobKnight*)tempObj)->SetState(&MobKnight::Delay);
		b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
		if (pos.x != 0 || pos.y != 0) {
			b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, TRUE, "mob");
		}
		else {
			b2Vec2 pos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE, "mob");
		}
		if (!Globals::chance(((Player*)(GameManager::GetInstance()->player))->EvadeChance))
		{
			SetState(&Player::GetHit);
			if (shield > 0 && !immune)
			{
				shield--;
			}
			else if (!immune)GetComponent<HP>()->Damage(1);
		}
		
	}

	if (strcmp(tempObj->name, "health_buff") == 0
		|| strcmp(tempObj->name, "void_shell_buff") == 0
		|| strcmp(tempObj->name, "shield_buff") == 0
		|| strcmp(tempObj->name, "magic_beat_buff") == 0
		|| strcmp(tempObj->name, "side_gun_buff") == 0
		|| strcmp(tempObj->name, "enhanced_muzzle_buff") == 0
		|| strcmp(tempObj->name, "blade_mail_buff") == 0
		|| strcmp(tempObj->name, "invincible_cloak_buff") == 0
		|| strcmp(tempObj->name, "swift_slipper_buff") == 0
		|| strcmp(tempObj->name, "enhanced_shell_buff") == 0)
	{
		((Perk*)(tempObj))->GetComponent<PowerUp>()->ActivatePower();
		GameManager::GetInstance()->spawnedBuffList.remove(tempObj);
		GameManager::GetInstance()->shopped = true;
		GameManager::GetInstance()->addToRemovalList(tempObj);
	}

	if (strcmp(tempObj->name, "ladder") == 0) {
		GameManager::GetInstance()->counter = 0;
		SetState(&Player::ClimbLadder);
	}

	if (strcmp(tempObj->name, "mine") == 0) {
		((Mine*)tempObj)->SetState(&Mine::Destroying);
	}
}


void Player::Update(float deltaTime)
{
	//Update HP GUI
	UpdateHP();
	UpdateShield();
	UpdateVoidShell();

	//Transition
	if (GetComponent<HP>()->dead) {
		SetState(&Player::Death);
	}

	if (activeState != NULL)
		(this->*activeState)();
	CalculateVelocity();
	calculateAngle();
	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	//cout << transform->position.x << " " << transform->position.y << endl;
	//cout << GetComponent<HP>()->health << " " << shield << endl;
	GameObject::Update(deltaTime);
}

void Player::LateUpdate(float deltaTime)
{
	if (fadeOut) {
		if (alpha > 0) {
			alpha -= fadeOut * deltaTime;
			if (alpha < 0) {
				alpha = 0;
				FadeOff();
			}

		}
	}

	if (fadeIn) {
		if (alpha < 1) {
			alpha += fadeIn * deltaTime;
			if (alpha > 1) {
				alpha = 1;
				FadeOff();
			}
		}
	}

	if (activeAnimation >= 0)
		animeList.at(activeAnimation)->alphaMod = alpha;
}

