#pragma once
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include <vector>

class ResourceManager
{
protected:
	ResourceManager() {}
	~ResourceManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	ResourceManager(const ResourceManager &) {}
	ResourceManager& operator =(const ResourceManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new ResourceManager;
	}
	static ResourceManager * GetInstance()
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

	void Init();
	void LoadEngineResources();
	int LoadNFG(char * filename);
	int LoadTGA(char * filename);
	int LoadShaders(char *verFile, char *fragFile);

	Model * GetSquare() { return GetModel(squareId); }
	Model * GetSquareTL() { return GetModel(squareTLId); }
	Model * GetModel(int modelID);
	Texture * GetTexture(int textureID);
	Shaders * GetShaders(int shadersID);

	void DeleteAll();

protected:
	static ResourceManager * ms_pInstance;

public:
	vector<Model *> modelMap{};
	vector<Texture *> textureMap{};
	vector<Shaders *> shaderMap{};

	int squareId = -1;
	int squareTLId = -1;
};