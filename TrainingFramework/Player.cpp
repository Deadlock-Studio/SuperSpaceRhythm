#include "stdafx.h"
#include "Player.h"
#include "Control.h"
#define iTICK 100
Player::Player()
{
	SetState(&Player::Spawn);

}

Player::Player(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : Player()
{
	name = _strdup(blueprint->name);

	//Update transform
	UpdatePosition(pos.x, pos.y, pos.z);
	UpdateRotation(rotation.x, rotation.y, rotation.z);
	UpdateScale(scale.x, scale.y, scale.z);

	//Clone components
	for (vector<Component *>::iterator it = blueprint->componentList.begin(); it != blueprint->componentList.end(); ++it) {
		AddComponent((*it)->Clone());
	}

	Init();
	iTick = iTICK;
}


Player::~Player()
{
	b2Body* tempBody = GetComponent<Collision2D>()->body;
	tempBody->GetWorld()->DestroyBody(tempBody);
}


void Player::Init()
{
	UpdateScale(1.5f, 1.5f, 1.0f);

	//filter MUST ALWAYS be the LAST THING to do in init
	b2Filter filter = GetComponent<Collision2D>()->body->GetFixtureList()->GetFilterData();
	//type of body
	filter.categoryBits = PLAYER;
	//collide with what
	filter.maskBits = BOSS | HEALTHPOTION| MOB | MOB_RED | MOB_BLUE | EXPLOSION | TNT_BOX | CRATE | WALL | TRAP | BULLET_E | SHIELD | MINE;
	GetComponent<Collision2D>()->body->GetFixtureList()->SetFilterData(filter);
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
		Vector3(transform->position.x, transform->position.y, 2),
		Vector3(2, 2, 2),
		Vector3());
	SetState(&Player::Idle);
}

void Player::Idle()
{
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
	if (iTick == 0) {
		SetState(&Player::Idle);	
		iTick = iTICK;
	}

}

void Player::Dashing()
{
	if (GetComponent<Control>()->dashTick == 0) {
		SetState(&Player::Idle);
	}
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
}

void Player::Death()
{
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
			SetState(&Player::GetHit);
		}
		//if player touch boss
		else {
			b2Vec2 pos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE, "boss");
			SetState(&Player::GetHit);
		}
	}
	if (strcmp(tempObj->name, "health_potion") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
	}
	if (strcmp(tempObj->name, "trap") == 0) {
		b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
		if (pos.x != 0 || pos.y != 0) {
			GetComponent<Control>()->Collide(pos.x, pos.y, TRUE, "trap");
			SetState(&Player::GetHit);

		}
		else {
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE,"trap");
			SetState(&Player::GetHit);
		}
	}
	if (strcmp(tempObj->name, "crate") == 0) {
		if (GetComponent<Control>()->isDashing) {
			((Crate*)tempObj)->SetState(&Crate::Destroying);
		}
	}
	if (strcmp(tempObj->name, "mob_shoot") == 0 || strcmp(tempObj->name, "mob_knight") == 0) {
		b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
		if (pos.x != 0 || pos.y != 0) {
			b2Vec2 pos = GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, TRUE, "mob");
			SetState(&Player::GetHit);
		}
		else {
			b2Vec2 pos = tempObj->GetComponent<Collision2D>()->body->GetLinearVelocity();
			GetComponent<Control>()->Collide(pos.x, pos.y, FALSE, "mob");
			SetState(&Player::GetHit);
		}
	}
	if (strcmp(tempObj->name, "mob_blue") == 0 
		|| strcmp(tempObj->name, "mob_red") == 0 
		|| strcmp(tempObj->name, "mob_white") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
		SetState(&Player::GetHit);
	}
	if (strcmp(tempObj->name, "explosion") == 0) {
		SetState(&Player::GetHit);
	}
	if (strcmp(tempObj->name, "eBullet") == 0) {
		SceneManager::GetInstance()->addToRemovalList(tempObj);
		SetState(&Player::GetHit);
	}
	if (strcmp(tempObj->name, "mob_knight") == 0) {
		((MobKnight*)(tempObj))->SetState(&MobKnight::Stop);
	}
	if (strcmp(tempObj->name, "mine") == 0) {
		((Mine*)tempObj)->SetState(&Mine::Destroying);

	}
}


void Player::Update(float deltaTime)
{
	b2Vec2 vec2 = GetComponent<Collision2D>()->body->GetPosition();
	if (activeState != NULL)
		(this->*activeState)();
	calculateAngle();
	b2Vec2 bodyPos = GetComponent<Collision2D>()->body->GetPosition();
	transform->setPosition(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, transform->position.z);
	GameObject::Update(deltaTime);
}

