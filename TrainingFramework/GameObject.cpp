#include "stdafx.h"
#include "Button.h"
#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
	transform->parentObj = this;
}

GameObject::GameObject(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) : GameObject()
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
}

GameObject::~GameObject()
{
	delete transform;
}

void GameObject::UpdateTransform(Vector3 position, Vector3 rotation, Vector3 scale)
{
	if (transform) {
		transform->UpdateTransform(position, rotation, scale);
	}
	if (GetComponent<Collision2D>()) {
		GetComponent<Collision2D>()->UpdateBodyPostition(position.x, position.y);
		GetComponent<Collision2D>()->UpdateBodyRotation(rotation.x);
		GetComponent<Collision2D>()->UpdateBodyScale(scale.x, scale.y);
	}
}

void GameObject::UpdatePosition(float x, float y, float z)
{
	if (transform) {
		transform->UpdatePosition(Vector3(x,y,z));
	}
	if (GetComponent<Collision2D>()) {
		GetComponent<Collision2D>()->UpdateBodyPostition(x, y);
	}
}

void GameObject::UpdateRotation(float x, float y, float z)
{
	if (transform) {
		transform->UpdateRotation(Vector3(x,y,z));
	}
	if (GetComponent<Collision2D>()) {
		GetComponent<Collision2D>()->UpdateBodyRotation(x);
	}
}

void GameObject::UpdateScale(float x, float y, float z)
{
	if (transform) {
		transform->UpdateScale(Vector3(x, y, z));
	}
	if (GetComponent<Collision2D>()) {
		GetComponent<Collision2D>()->UpdateBodyScale(x, y);
	}
}

void GameObject::AddComponent(Component * comp) {
	Blueprint::AddComponent(comp);
	comp->parentObj = this;
	comp->parentTrans = this->transform;

	if (dynamic_cast<Collision2D *>(comp) != NULL) {
		((Collision2D*)comp)->Init();
	}

	if (dynamic_cast<Button *>(comp) != NULL) {
		((Button*)comp)->Init();
	}
}

void GameObject::AddToPosition(float x, float y)
{
	b2Body * tempBody = GetComponent<Collision2D>()->body;
	b2Vec2 posOffset;
	posOffset.x = x / PIXEL_RATIO;
	posOffset.y = y / PIXEL_RATIO;
	tempBody->SetLinearVelocity(posOffset);
	b2Vec2 bodyPos = tempBody->GetPosition();
	if (strcmp(name, "eBullet") == 0 || strcmp(name, "pBullet_red") == 0 || strcmp(name, "pBullet_blue") == 0) {
		transform->UpdatePosition(Vector3(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, EFFECT_LAYER));
	}
	else if (strcmp(name, "player") == 0 || strcmp(name, "boss") == 0) {
		transform->UpdatePosition(Vector3(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, PLAYER_LAYER));
	}
	else transform->UpdatePosition(Vector3(bodyPos.x * PIXEL_RATIO, bodyPos.y * PIXEL_RATIO, MOB_LAYER));
}

void GameObject::checkCollision(GameObject * tempObj)
{
}

void GameObject::Init()
{
}

void GameObject::Update(float deltaTime)
{
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		if ((*it)->isActive)
			(*it)->Update(deltaTime);
	}
}

void GameObject::LateUpdate(float deltaTime)
{
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		if ((*it)->isActive)
			(*it)->LateUpdate(deltaTime);
	}
}

void GameObject::Draw()
{
	for (std::vector<Component *>::iterator it = componentList.begin(); it != componentList.end(); ++it) {
		if ((*it)->isActive)
			(*it)->Draw();
	}
}