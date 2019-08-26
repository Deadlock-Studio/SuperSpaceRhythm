#pragma once
#include "Shaders.h"
#include "Camera.h"
#include "Collision2D.h"
#include "Blueprint.h"
#include "PowerUp.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Animation.h"
#include "GUI.h"
#include "Font.h"
#include "HP.h"
#include "Script.h"
#include <list>
using namespace std;

class SceneManager
{
protected:
	SceneManager() {}
	~SceneManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	SceneManager(const SceneManager &) {}
	SceneManager& operator =(const SceneManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new SceneManager;
	}
	static SceneManager * GetInstance()
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

	/*
	* File read methods
	*/
	void Init();
	void LoadScene(char * filename);
	void GetComponent(ifstream & file, Blueprint * blueprint);
	Camera * GetCam(ifstream & file);
	char * GetName(ifstream & file);
	void GetTransform(ifstream & file, GameObject * obj);
	SpriteRenderer * GetSprite(ifstream & file);
	Animation * GetAnim(ifstream & file);
	Collision2D * GetCollision(ifstream & file);
	PowerUp * GetPowerUp(ifstream & file);
	HP * GetHP(ifstream & file);

	/*
	* Update and Draw
	*/
	void Draw();
	void UnloadAll();
	void UnloadObjects();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);

	void safeDel_k(GameObject* key);
	void addToRemovalList(GameObject* key);
	void deleteRemovalList();
	void UseCamera(int camNum);

	/*
	* Blueprint, object methods
	*/
	GameObject* SpawnObject(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	GUI* SpawnGUI(Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation);
	GameObject* SpawnBullet(float x, float y, float mX, float mY, char * type);
	GameObject * SpawnBomb(float x, float y, float mX, float mY);
	Blueprint* GetBlueprintByName(char* name);
	GameObject* GetObjectByName(char* name);
	Script* GetScriptByName(char* name);

protected:
	static SceneManager * ms_pInstance;

public:
	Shaders * shaders;
	Shaders * fontShaders;
	list<Blueprint *> blueprintList;
	list<GameObject*> objectList;
	vector<Camera *> cameraList;
	vector<Script*> scriptList;
	list<GameObject*> removalList;
	Camera * usedCamera = NULL;
	char * sceneName;
};