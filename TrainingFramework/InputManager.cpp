#include "stdafx.h"

InputManager * InputManager::ms_pInstance = NULL;

void InputManager::LoadDefault()
{
	keyBind.insert({ 'W', MoveForward});
	keyBind.insert({ 'S', MoveBackWard });
	keyBind.insert({ 'A', MoveLeft });
	keyBind.insert({ 'D', MoveRight });
	keyBind.insert({ 'E', BeatIt });
	keyBind.insert({ 'F', Reform });
	keyBind.insert({ ' ', Dash });
	keyBind.insert({ 'B', DebugMode });
}

void InputManager::keyUpdate(unsigned char key, bool bIsPressed)
{
	if (keyBind.count(key) == 0) return;
	int keyMask = 1 << (int)keyBind[key];
	keyMap = bIsPressed ? (keyMap | keyMask) : (keyMap & ~keyMask);
}

bool InputManager::ActionCheck(Actions action)
{
	int keyMask = 1 << (int)action;
	return (keyMap & keyMask) != 0;
}

void InputManager::MouseDown(float x, float y)
{
	isMouseDown = true;
	mX = x;
	mY = y;
}

void InputManager::MouseUp(float x, float y)
{
	isMouseDown = false;
}

void InputManager::MousePosition(float x, float y)
{
	mouseX = x;
	mouseY = y;
}

