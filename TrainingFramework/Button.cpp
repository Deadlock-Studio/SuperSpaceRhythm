#include "stdafx.h"
#include "Button.h"


Button::Button()
{
}


Button::~Button()
{
}

void Button::Init()
{
	sprite = ResourceManager::GetInstance()->GetTexture(((GameObject*)parentObj)->GetComponent<SpriteRenderer>()->textureId);
}

Component * Button::Clone()
{
	return new Button();
}

void Button::Update(float deltaTime)
{
	if (!InputManager::GetInstance()->isMouseDown) {
		isClicked = false;
		return;
	}
	Vector3 camPosition = SceneManager::GetInstance()->usedCamera->position;
	//Check if mouse clicked this object's sprite or not
	Vector2 screenPosition = Vector2(((Transform*)parentTrans)->position.x - camPosition.x, ((Transform*)parentTrans)->position.y - camPosition.y);
	Vector2 mousePos = Vector2(InputManager::GetInstance()->mX, InputManager::GetInstance()->mY);
	
	Vector2 bottomLeft = Vector2(screenPosition.x - sprite->width/2, screenPosition.y - sprite->height / 2);
	Vector2 topRight = Vector2(screenPosition.x + sprite->width/2, screenPosition.y + sprite->height / 2);

	if (mousePos.x > bottomLeft.x && mousePos.x < topRight.x && mousePos.y > bottomLeft.y && mousePos.y < topRight.y)
		isClicked = true;
}
