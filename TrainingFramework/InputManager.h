#pragma once
//Movement
//
#define W_MASK (1 << 0)
#define A_MASK (1 << 1)
#define S_MASK (1 << 2)
#define D_MASK (1 << 3)
#define SPACE_MASK (1 << 4)
#define SHIFT_MASK (1 << 5)

//Speed
#define CTRL_MASK (1 << 6)

//Reset
#define R_MASK (1 << 7)

//Beat hit test
#define E_MASK (1 << 8)

class InputManager
{
protected:
	InputManager() {}
	~InputManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	InputManager(const InputManager &) {}
	InputManager& operator =(const InputManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new InputManager;
	}
	static InputManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	void keyUpdate(unsigned char key, bool bIsPressed);
	bool keyDown(unsigned char key);

protected:
	static InputManager * ms_pInstance;

public:
	int keyMap = 0;
};