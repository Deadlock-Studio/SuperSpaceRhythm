#pragma once
#include "Text.h"
#include "Room.h"
#include "PerkDetails.h"
#include <string>
#include <shellapi.h>

typedef struct RoomDetails {
	char* roomName;
	int timeUsed;
};

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
			GetInstance()->UnloadAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	void Init();
	void LoadBuffList();
	void LoadRoomList();
	void LoadConstructorMap();
	void Update(float deltaTime);
	GameObject* Spawn(string constructorName, Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	GameObject* SpawnToRoom(string constructorName, Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	void ResetRoomAndBuff();
	void UnloadObjects();
	void UnloadAll();
	void addToRemovalList(GameObject* key);

	/*
	* GameStates
	*/
	void SetState(void(GameManager::*state)(float)) {
		activeState = state;
	}
	void Loading(float deltaTime);
	void SplashScreen(float deltaTime);
	void Menu(float deltaTime);
	void VictoryScreen(float deltaTime);
	void PreOrder(float deltaTime);
	void Tutorial(float deltaTime);
	void Option(float deltaTime);
	void LoadDebugRoom(float deltaTime);
	void LoadCalibrate(float deltaTime);
	void LoadRoom(float deltaTime);
	void RoomStart(float deltaTime);
	void Victory(float deltaTime);
	void Defeat(float deltaTime);
	void UnloadRoom(float deltaTime);
	void(GameManager::*activeState)(float) = NULL;
protected:
	static GameManager * ms_pInstance;

public:
	//Splash screen
	GameObject* splashScreen;

	//Menu
	GameObject* playButton;
	GameObject* helpButton;
	GameObject* optionsButton;
	GameObject* mascot;
	GameObject* tutorial;
	GameObject* menuBG;
	GameObject* gameBG;
	GameObject* backButton;
	GameObject* calibrateButton;
	GameObject* musicPlus;
	GameObject* musicMinus;

	Text* title;
	Text* victoryText;
	Text* defeatText;
	Text* musicVolText;
	GameObject* victoryScreen;
	GameObject* preOrderButton;
	bool doneVictory = false;

	Text* calBeat;
	Text* calComplete;
	Text* musicVolumeHolder;	//GUI
	vector<GameObject*> healthBar;
	vector<GameObject*> halfHealthBar;
	vector<GameObject*> emptyHealthBar;
	vector<GameObject*> shieldBar;
	GameObject* voidShell;

	//Shop
	GameObject* shopKeeper;
	GameObject* ladder;
	bool shopped = false;
	bool shopSpawned = false;
	int buffPerShop = 3;
	float buffOffset = 100.0f;
	void SpawnBuff();
	list<GameObject*> spawnedBuffList;

	char* musicVolume = "0";
	
	void HideAllGUI();
	void HideHealthBar();

	//Game state
	bool playPressed = false;
	bool optionPressed = false;
	bool helpPressed = false;
	bool backPressed = false;
	bool calPressed = false;
	bool calibrate = false;
	bool musicPlusPressed = false;
	bool musicMinusPressed = false;


	//Rooms
	float counter = 0;
	int roomNum = 0;
	char* bossRoom = "boss_room";
	RoomDetails roomList[9];

	//Spawn algor
	float clamp_X(float x);
	float clamp_Y(float y);

	int scriptIter = 0;
	int mobCount = 0;
	Script* script = NULL;
	Camera * camera;
	GameObject* room;
	GameObject * player = NULL;
	list<GameObject*> roomObject;
	vector<PerkDetails*> perkList;
	map<string, GameObject*(*)(Blueprint*, Vector3, Vector3, Vector3)> constructorMap;
};