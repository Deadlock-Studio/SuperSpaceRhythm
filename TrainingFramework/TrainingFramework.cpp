// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>

int keyMap = 0;
bool hit = FALSE;

int Init ( ESContext *esContext )
{
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	//Singleton Inits
	SceneManager::CreateInstance();
	ResourceManager::CreateInstance();
	InputManager::CreateInstance();
	SoundManager::CreateInstance();

	//Load scene
	ResourceManager::GetInstance()->LoadEngineResources();
	SceneManager::GetInstance()->LoadScene("../Resources/Scenes/Demo5.scn");
	SceneManager::GetInstance()->UseCamera(0);
	//SceneManager::GetInstance()->PrintAll();

	//Load sound
	SoundManager::GetInstance()->Init();
	//SoundManager::GetInstance()->getTrack("cinderella_step")->music.play();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	return 0;
}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SceneManager::GetInstance()->DrawAll();

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	SceneManager::GetInstance()->Update(deltaTime);

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	InputManager::GetInstance()->keyUpdate(key, bIsPressed);
}

void CleanUp()
{
	//Singleton Destroys
	InputManager::DestroyInstance();
	SceneManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	SoundManager::DestroyInstance();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	MemoryDump();
	printf("Press any key...\n");
	_getch();

	return 0;
}

