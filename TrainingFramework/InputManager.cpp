#include "stdafx.h"

InputManager * InputManager::ms_pInstance = NULL;

void InputManager::LoadDefault()
{
	keyBind.insert({ 'W', MoveForward});
	keyBind.insert({ 'S', MoveBackWard });
	keyBind.insert({ 'A', MoveLeft });
	keyBind.insert({ 'D', MoveRight });
	keyBind.insert({ 'E', BeatIt });
}

void InputManager::keyUpdate(unsigned char key, bool bIsPressed)
{
	int keyMask = 1 << (int)keyBind[key];
	keyMap = bIsPressed ? (keyMap | keyMask) : (keyMap & ~keyMask);
}

bool InputManager::ActionCheck(Actions action)
{
	int keyMask = 1 << (int)action;
	return keyMap & keyMask;
}
