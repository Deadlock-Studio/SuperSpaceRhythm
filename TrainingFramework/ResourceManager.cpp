#include "stdafx.h"
#include <iostream>
#include "ResourceManager.h"
#include "HandyString.h"
#include <algorithm>
#include <cctype>

ResourceManager * ResourceManager::ms_pInstance = NULL;

void ResourceManager::Init()
{
	LoadEngineResources();
}

void ResourceManager::LoadEngineResources()
{
	squareId = ResourceManager::GetInstance()->LoadNFG("../Resources/Models/Square.nfg");
	squareTLId = ResourceManager::GetInstance()->LoadNFG("../Resources/Models/FontSquare.nfg");
}

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

Model * ResourceManager::GetModel(int modelID)
{
	return modelMap.at(modelID);
}

Texture * ResourceManager::GetTexture(int textureID)
{
	return textureMap.at(textureID);
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
	modelMap.clear();

	for (std::vector<Texture*>::iterator it = textureMap.begin(); it != textureMap.end(); ++it) {
		delete *it;
	}
	textureMap.clear();

	for (std::vector<Shaders*>::iterator it = shaderMap.begin(); it != shaderMap.end(); ++it) {
		delete *it;
	}
	shaderMap.clear();
}

