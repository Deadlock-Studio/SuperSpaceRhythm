// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>

int keyMap = 0;
int signal = 0;

int Init(ESContext *esContext)
{
	glClearColor(0.1451f, 0.075f, 0.102f, 1.0f);
	srand(time(NULL));

	//Singleton Creates
	TextManager::CreateInstance();
	PhysicManager::CreateInstance();
	SceneManager::CreateInstance();
	ResourceManager::CreateInstance();
	InputManager::CreateInstance();
	SoundManager::CreateInstance();
	GameManager::CreateInstance();

	//Singleton Inits
	TextManager::GetInstance()->Init();
	PhysicManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	GameManager::GetInstance()->Init();
	ResourceManager::GetInstance()->Init();
	InputManager::GetInstance()->Init();
	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->currentlyPlaying = SoundManager::GetInstance()->getTrack("level");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SceneManager::GetInstance()->Draw();
	TextManager::GetInstance()->Draw();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext *esContext, float deltaTime)
{
	//Frametime counter
	//cout << "frametime " << deltaTime << endl;
	SoundManager::GetInstance()->signalPass = SoundManager::GetInstance()->RhythmConductor(SoundManager::GetInstance()->currentlyPlaying, deltaTime);
	SceneManager::GetInstance()->Update(deltaTime);
	GameManager::GetInstance()->Update(deltaTime);
	PhysicManager::GetInstance()->Update(deltaTime);
	TextManager::GetInstance()->Update(deltaTime);

	if (GameManager::GetInstance()->calibrate)
	{
		if (!SoundManager::GetInstance()->audioCalibrated)
		{
			SoundManager::GetInstance()->CalibrateAudio();
		}
		else if (!SoundManager::GetInstance()->visualCalibrated)
		{
			SoundManager::GetInstance()->CalibrateVisual();
		}
	}

	//Always bottom
	SceneManager::GetInstance()->LateUpdate(deltaTime);
	TextManager::GetInstance()->LateUpdate(deltaTime);
}

void Key(ESContext *esContext, unsigned char key, bool bIsPressed)
{
	InputManager::GetInstance()->keyUpdate(key, bIsPressed);
}

void TouchActionMove(ESContext *esContext, int x, int y)
{
	InputManager::GetInstance()->MousePosition((float)x, (float)y);
}

void TouchActionUp(ESContext *esContext, int x, int y)
{
	InputManager::GetInstance()->MouseUp((float)x, (float)y);
}

void TouchActionDown(ESContext *esContext, int x, int y)
{
	InputManager::GetInstance()->MouseDown((float)x, (float)y);

}
void RightMouseUp(ESContext *esContext, int x, int y)
{
	InputManager::GetInstance()->RightMouseUp((float)x, (float)y);
}

void RightMouseDown(ESContext *esContext, int x, int y)
{
	InputManager::GetInstance()->RightMouseDown((float)x, (float)y);

}

void CleanUp()
{
	//Singleton Destroys
	InputManager::DestroyInstance();
	GameManager::DestroyInstance();
	SceneManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	SoundManager::DestroyInstance();
	PhysicManager::DestroyInstance();
	TextManager::DestroyInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Super Space Rhythm", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseDownFunc(&esContext, TouchActionDown);
	esRegisterMouseUpFunc(&esContext, TouchActionUp);
	esRegisterRightMouseDownFunc(&esContext, RightMouseDown);
	esRegisterRightMouseUpFunc(&esContext, RightMouseUp);
	esRegisterMouseMoveFunc(&esContext, TouchActionMove);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

