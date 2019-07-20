// TrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include <conio.h>
#include "Model.h"
#include "TrainingFramework.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>

int keyMap = 0;

int Init ( ESContext *esContext )
{
	glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );

	SceneManager::CreateInstance();
	ResourceManager::CreateInstance();

	//Load scene
	ResourceManager::GetInstance()->LoadEngineResources();
	SceneManager::GetInstance()->LoadScene("../Resources/Scenes/Demo5.scn");
	SceneManager::GetInstance()->UseCamera(0);
	//SceneManager::GetInstance()->PrintAll();

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

	//TODO control component
	Camera * cam = SceneManager::GetInstance()->usedCamera;
	Vector3 zero = Vector3(0, 0, 0);

	if (keyMap & R_MASK) {
		cam->position = Vector3(0.0, 1.5, 5.0);
		cam->cameraDirection = Vector3(0.0, 0.0, -1.0);
		cam->cameraUp = Vector3(0.0, 1.0, 0.0);
		cam->UpdateCamera();
		return;
	}

	Vector3 moveVector = Vector3(0, 0, 0);
	float rotate[3] = { 0,0,0 };
	moveVector += ((keyMap & W_MASK) ? cam->cameraDirection : zero) + ((keyMap & S_MASK) ? -cam->cameraDirection : zero);
	moveVector += ((keyMap & D_MASK) ? cam->cameraRight : zero) + ((keyMap & A_MASK) ? -cam->cameraRight : zero);
	moveVector += ((keyMap & SPACE_MASK) ? cam->cameraUp : zero) + ((keyMap & SHIFT_MASK) ? -cam->cameraUp : zero);

	float speed = keyMap & CTRL_MASK ? 2 * cam->speed : cam->speed;

	cam->position += (moveVector * speed * deltaTime);

	int rotateX = ((keyMap & K_MASK) ? -1 : 0) + ((keyMap & I_MASK) ? 1 : 0);
	cam->cameraDirection.RotateAxis(rotateX * cam->rotSpeed * deltaTime, cam->cameraRight);
	int rotateY = ((keyMap & L_MASK) ? -1 : 0) + ((keyMap & J_MASK) ? 1 : 0);
	cam->cameraDirection.RotateAxis(rotateY * cam->rotSpeed * deltaTime, cam->cameraUp);
	int rotateZ = ((keyMap & U_MASK) ? -1 : 0) + ((keyMap & O_MASK) ? 1 : 0);
	cam->cameraUp.RotateAxis(rotateZ * cam->rotSpeed * deltaTime, cam->cameraDirection);

	cam->UpdateCamera();
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
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
	//Look
	else if (key == 'I')
		keyMap = bIsPressed ? (keyMap | I_MASK) : (keyMap & ~I_MASK);
	else if (key == 'J')
		keyMap = bIsPressed ? (keyMap | J_MASK) : (keyMap & ~J_MASK);
	else if (key == 'K')
		keyMap = bIsPressed ? (keyMap | K_MASK) : (keyMap & ~K_MASK);
	else if (key == 'L')
		keyMap = bIsPressed ? (keyMap | L_MASK) : (keyMap & ~L_MASK);
	else if (key == 'U')
		keyMap = bIsPressed ? (keyMap | U_MASK) : (keyMap & ~U_MASK);
	else if (key == 'O')
		keyMap = bIsPressed ? (keyMap | O_MASK) : (keyMap & ~O_MASK);
	//Speed
	else if (key == 17)
		keyMap = bIsPressed ? (keyMap | CTRL_MASK) : (keyMap & ~CTRL_MASK);
	//Reset
	else if (key == 'R')
		keyMap = bIsPressed ? (keyMap | R_MASK) : (keyMap & ~R_MASK);
}

void CleanUp()
{
	SceneManager::DestroyInstance();
	ResourceManager::DestroyInstance();
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

