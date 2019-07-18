#pragma once
#include <vector>
#include "Object.h"
#include "Camera.h"

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
			GetInstance()->DeleteAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	void LoadScene(char * filename);
	void PrintAll();
	void DrawAll();
	void DeleteAll();
	void UseCamera(int camNum);
	void Update(float deltaTime);

protected:
	static SceneManager * ms_pInstance;

public:
	float time = 0;
	std::vector<Object *> objectList;
	std::vector<Camera *> cameraList;
	Camera * usedCamera = NULL;
	char * sceneName;
};