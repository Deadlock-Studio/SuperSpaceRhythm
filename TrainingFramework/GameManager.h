#pragma once
#include "Text.h"
#include "Room.h"

template <class T> GameObject* constructor(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation) { return (GameObject*)(new T(blueprint, pos, scale, rotation)); }

class GameManager
{
protected:
	GameManager() {}
	~GameManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	GameManager(const GameManager &) {}
	GameManager& operator =(const GameManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new GameManager;
	}
	static GameManager * GetInstance()
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

	void Init();
	void LoadConstructorMap();
	void Update(float deltaTime);
	GameObject* Spawn(string constructorName, Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);

	/*
	* GameStates
	*/
	void SetState(void(GameManager::*state)(float)) {
		activeState = state;
	}
	void Loading(float deltaTime);
	void SplashScreen(float deltaTime);
	void Menu(float deltaTime);
	void LoadDebugRoom(float deltaTime);
	void RoomStart(float deltaTime);
	void LoadRoom(float deltaTime);
	void(GameManager::*activeState)(float) = NULL;
protected:
	static GameManager * ms_pInstance;

public:
	//Splash screen
	GameObject* splashScreen;

	//Menu
	GameObject* playButton;
	GameObject* optionsButton;
	GameObject* menuBG;
	Text* title;
	
	void UnloadAll();

	//Game state
	bool playPressed = false;

	//GUI
	float time = 0;

	GameObject* room;
	Camera * camera;
	GameObject * player;
	map<string, GameObject*(*)(Blueprint*, Vector3, Vector3, Vector3)> constructorMap;
};