#include "stdafx.h"

InputManager * InputManager::ms_pInstance = NULL;

void InputManager::keyUpdate(unsigned char key, bool bIsPressed)
{
	//Move
	if (key == 'W')
		keyMap = bIsPressed ? (keyMap | W_MASK) : (keyMap & ~W_MASK);
	else if (key == 'A')
		keyMap = bIsPressed ? (keyMap | A_MASK) : (keyMap & ~A_MASK);
	else if (key == 'S')
		keyMap = bIsPressed ? (keyMap | S_MASK) : (keyMap & ~S_MASK);
	else if (key == 'D')
		keyMap = bIsPressed ? (keyMap | D_MASK) : (keyMap & ~D_MASK);
	else if (key == ' ')
		keyMap = bIsPressed ? (keyMap | SPACE_MASK) : (keyMap & ~SPACE_MASK);
	else if ((int)key == 16)
		keyMap = bIsPressed ? (keyMap | SHIFT_MASK) : (keyMap & ~SHIFT_MASK);
	//Speed
	else if (key == 17)
		keyMap = bIsPressed ? (keyMap | CTRL_MASK) : (keyMap & ~CTRL_MASK);
	//Reset
	else if (key == 'R')
		keyMap = bIsPressed ? (keyMap | R_MASK) : (keyMap & ~R_MASK);

	//Beat hit test
	else if (key == 'E')
		keyMap = bIsPressed ? (keyMap | E_MASK) : (keyMap & ~E_MASK);
}

bool InputManager::keyDown(unsigned char key)
{
	//Move
	if (key == 'W')
		return keyMap & W_MASK;
	else if (key == 'A')
		return keyMap & A_MASK;
	else if (key == 'S')
		return keyMap & S_MASK;
	else if (key == 'D')
		return keyMap & D_MASK;
	else if (key == ' ')
		return keyMap & SPACE_MASK;
	else if ((int)key == 16)
		return keyMap & SHIFT_MASK;
	//Speed
	else if (key == 17)
		return keyMap & CTRL_MASK;
	//Reset
	else if (key == 'R')
		return keyMap & R_MASK;
	//Beat hit test
	else if (key == 'E')
		return keyMap & E_MASK;
}
