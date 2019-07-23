// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include <conio.h>

int keyMap = 0;
bool hit = FALSE;
float timer = 0;
int frame = 0;

int Init ( ESContext *esContext )
{
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	//Singleton Inits
	SceneManager::CreateInstance();
	ResourceManager::CreateInstance();
	InputManager::CreateInstance();
	SoundManager::CreateInstance();
	InputManager::CreateInstance();

	//Load scene
	ResourceManager::GetInstance()->LoadEngineResources();
	SceneManager::GetInstance()->LoadScene("../Resources/Templates/Template1.txt");
	SceneManager::GetInstance()->UseCamera(0);
	//SceneManager::GetInstance()->PrintAll();
	SceneManager::GetInstance()->SpawnObject(SceneManager::GetInstance()->getObjectInstance("player"), 0, 0, 1);

	SceneManager::GetInstance()->SpawnObject(SceneManager::GetInstance()->getObjectInstance("beat"), 70, 35, 1);
	SceneManager::GetInstance()->SpawnObject(SceneManager::GetInstance()->getObjectInstance("beat"), 10, 35, 1);
	SceneManager::GetInstance()->SpawnObject(SceneManager::GetInstance()->getObjectInstance("grid"), 70, 35, 1);


	//Load sound
	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->getTrack("cinderella_step")->music.play();
	SoundManager::GetInstance()->getTrack("cinderella_step")->music.setVolume(0);
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
	frame += 1;
	SceneManager::GetInstance()->Update(deltaTime);
	//SoundManager::GetInstance()->BeatKeeper(SoundManager::GetInstance()->getTrack("cinderella_step"));
	timer += deltaTime;
	if (timer > 1) {
		printf("fps: %d\n", frame);
		timer = 0;
		frame = 0;
		sf::Time temp = SoundManager::GetInstance()->getTrack("cinderella_step")->music.getPlayingOffset();
		printf("song pos: %f\n", temp.asSeconds());
	}
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
	InputManager::DestroyInstance();
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

