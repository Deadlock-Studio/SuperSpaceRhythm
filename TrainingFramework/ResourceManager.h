#pragma once
#include <vector>
#include "Object.h"

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
		for (int i = 0; i < 128 * 128; i++)
			GetInstance()->heightmap[i] = 0;
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

	int LoadNFG(char * filename);
	int LoadTGA(char * filename);
	int LoadShaders(char *verFile, char *fragFile);
	int LoadCubeTGA(char * filename);
	int LoadRaw(char * filename);
	Model * GetModel(int modelID);
	Texture * GetTexture(int textureID);
	CubeTexture * GetCubeTexture(int cubetextID);
	Shaders * GetShaders(int shadersID);
	void DeleteAll();

protected:
	static ResourceManager * ms_pInstance;

public:
	float height = 10.0f;
	BYTE heightmap[128*128];
	std::vector<Model *> modelMap;
	std::vector<Texture *> textureMap;
	std::vector<Shaders *> shaderMap;
	std::vector<CubeTexture *> cubeTextMap;
};