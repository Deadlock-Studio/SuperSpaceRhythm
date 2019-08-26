#include "stdafx.h"
#include "Pawn.h"


Pawn::Pawn()
{
	SetState(&Pawn::Idle);
}


Pawn::~Pawn()
{
}

void Pawn::AddComponent(Component * comp)
{
	GameObject::AddComponent(comp);
	if (dynamic_cast<Animation *>(comp) != NULL) {
		comp->isActive = false;
		animeList.push_back((Animation*)comp);
	}
}

void Pawn::PlayAnimation(int key)
{
	//Deactivate active animation and active this one if its not active
	if (!animeList.at(key)->isActive) {
		animeList.at(key)->isActive = true;
		if (activeAnimation != -1)
			animeList.at(activeAnimation)->isActive = false;
		activeAnimation = key;
	}
}

void Pawn::Idle()
{
	//Transition
	//Control* control = GetComponent<Control>();
	//if (control->ActionCheck(MoveLeft))
	//	SetState(&Pawn::WalkLeft);
	//else if (control->ActionCheck(MoveRight))
	//	SetState(&Pawn::WalkRight);

	//State execution
	PlayAnimation(0);
}

void Pawn::WalkLeft()
{
	//Transition
	//Control* control = GetComponent<Control>();
	//if (!control->ActionCheck(MoveLeft))
	//	SetState(&Pawn::Idle);

	//State execution
	PlayAnimation(1);
}

void Pawn::WalkRight()
{
	//Transition
	//Control* control = GetComponent<Control>();
	//if (!control->ActionCheck(MoveRight))
	//	SetState(&Pawn::Idle);

	//State execution
	PlayAnimation(2);
}
