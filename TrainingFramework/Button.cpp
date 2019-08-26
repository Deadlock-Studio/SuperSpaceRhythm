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
	Vector3 camPosition = SceneManager::GetInstance()->usedCamera->position;
	Vector2 screenPosition = Vector2(((Transform*)parentTrans)->position.x - camPosition.x, ((Transform*)parentTrans)->position.y - camPosition.y);

	Vector2 bottomLeft = Vector2(screenPosition.x - sprite->width / 2, screenPosition.y - sprite->height / 2);
	Vector2 topRight = Vector2(screenPosition.x + sprite->width / 2, screenPosition.y + sprite->height / 2);

	//Check if mouse hovering over this object's sprite or not
	Vector2 MouseHover = Vector2(InputManager::GetInstance()->mouseX, Globals::screenHeight - InputManager::GetInstance()->mouseY);
	if (MouseHover.x > bottomLeft.x && MouseHover.x < topRight.x && MouseHover.y > bottomLeft.y && MouseHover.y < topRight.y)
		isHovered = true;
	else isHovered = false;

	//Check if mouse clicked this object's sprite or not
	if (!InputManager::GetInstance()->isMouseDown) {
		isClicked = false;
		return;
	}
	else {
		Vector2 mouseDown = Vector2(InputManager::GetInstance()->mX, Globals::screenHeight - InputManager::GetInstance()->mY);
		if (mouseDown.x > bottomLeft.x && mouseDown.x < topRight.x && mouseDown.y > bottomLeft.y && mouseDown.y < topRight.y)
			isClicked = true;
	}
}
