#include "stdafx.h"
#include <iostream>
#include "ResourceManager.h"
#include "HandyString.h"
#include <algorithm>
#include <cctype>

ResourceManager * ResourceManager::ms_pInstance = NULL;

int ResourceManager::LoadNFG(char * filename)
{
	for (std::vector<Model*>::iterator it = modelMap.begin(); it != modelMap.end(); ++it) {
		if (!strcmp((*it)->filename, filename)) return std::distance(modelMap.begin(), it);
	}

	Model * model = new Model(filename);
	modelMap.push_back(model);

	return modelMap.size()-1;
}

int ResourceManager::LoadTGA(char * filename)
{
	for (std::vector<Texture*>::iterator it = textureMap.begin(); it != textureMap.end(); ++it) {
		if (!strcmp((*it)->filename, filename)) return std::distance(textureMap.begin(), it);
	}

	Texture * texture = new Texture(filename);
	textureMap.push_back(texture);

	return textureMap.size() - 1;
}

int ResourceManager::LoadShaders(char * verFile, char * fragFile) {
	for (std::vector<Shaders*>::iterator it = shaderMap.begin(); it != shaderMap.end(); ++it) {
		if (!strcmp((*it)->verFile, verFile) && !strcmp((*it)->fragFile, fragFile)) return std::distance(shaderMap.begin(), it);
	}

	Shaders * shaders = new Shaders(verFile, fragFile);
	shaderMap.push_back(shaders);

	return shaderMap.size() - 1;
}

int getPositionFromName(char * filename) {
	string name = HandyString::getFileName(string(filename));
	std::transform(name.begin(), name.end(), name.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (!name.compare("right")) return 0;
	else if (!name.compare("left")) return 1;
	else if (!name.compare("top")) return 2;
	else if (!name.compare("down")) return 3;
	else if (!name.compare("back")) return 4;
	else if (!name.compare("front")) return 5;
	return -1;
}

int ResourceManager::LoadCubeTGA(char * filename)
{
	string parent = HandyString::getFileParent(string(filename));
	for (std::vector<CubeTexture*>::iterator it = cubeTextMap.begin(); it != cubeTextMap.end(); ++it) {
		if (!parent.compare((*it)->parentName)) {
			(*it)->AddTexture(filename, getPositionFromName(filename));
			return std::distance(cubeTextMap.begin(), it);
		}
	}

	CubeTexture * cube = new CubeTexture(strdup(parent.c_str()));
	cubeTextMap.push_back(cube);
	cube->AddTexture(filename, getPositionFromName(filename));

	return cubeTextMap.size() - 1;
}

int ResourceManager::LoadRaw(char * filename)
{
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen(filename, "rb");

	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		std::cout << "Can't Find The Height Map!\n";
		return -1;
	}
	// Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread(heightmap, 1, 128*128, pFile);

	// After We Read The Data, It's A Good Idea To Check If Everything Read Fine
	int result = ferror(pFile);

	// Check If We Received An Error
	if (result)
	{
		std::cout << "Failed To Get Data!\n";
	}

	// Close The File
	fclose(pFile);
	return 0;
}

Model * ResourceManager::GetModel(int modelID)
{
	return modelMap.at(modelID);
}

Texture * ResourceManager::GetTexture(int textureID)
{
	return textureMap.at(textureID);
}

CubeTexture * ResourceManager::GetCubeTexture(int cubetextID)
{
	return cubeTextMap.at(cubetextID);
}

Shaders * ResourceManager::GetShaders(int shadersID)
{
	return shaderMap.at(shadersID);
}

void ResourceManager::DeleteAll()
{
	for (std::vector<Model*>::iterator it = modelMap.begin(); it != modelMap.end(); ++it) {
		delete *it;
	}

	for (std::vector<Texture*>::iterator it = textureMap.begin(); it != textureMap.end(); ++it) {
		delete *it;
	}

	for (std::vector<CubeTexture*>::iterator it = cubeTextMap.begin(); it != cubeTextMap.end(); ++it) {
		delete *it;
	}

	for (std::vector<Shaders*>::iterator it = shaderMap.begin(); it != shaderMap.end(); ++it) {
		delete *it;
	}
}

