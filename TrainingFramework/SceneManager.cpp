#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;

SceneManager * SceneManager::ms_pInstance = NULL;

void SceneManager::LoadScene(char * filename)
{
	shaders = new Shaders("../Resources/Shaders/CommonShaderVS.vs", 
		"../Resources/Shaders/CommonShaderFS.fs");

	sceneName = strdup(HandyString::getFileName(string(filename)).c_str());

	string line;
	ifstream file(filename);
	if (file.is_open())
	{
		while (getline(file, line)) {
			//TODO check invalid input
			int id;
			if (line.compare("") == 0) continue;
			if (sscanf(line.c_str(), "CAMERA %d", &id)) {
				//New Camera
				Camera * cam = new Camera();

				float x;
				getline(file, line);
				sscanf(line.c_str(), "NEAR: %f", &x);
				cam->nearPlane = x;
				getline(file, line);
				sscanf(line.c_str(), "FAR: %f", &x);
				cam->farPlane = x;
				getline(file, line);
				sscanf(line.c_str(), "FOV: %f", &x);
				cam->fov = x;
				getline(file, line);
				sscanf(line.c_str(), "SPEED: %f", &x);
				cam->speed = x;
				getline(file, line);
				sscanf(line.c_str(), "ROTSPEED: %f", &x);
				cam->rotSpeed = x;

				cam->InitCamera();
				cameraList.push_back(cam);
			}
			else if (sscanf(line.c_str(), "OBJECT %d", &id)) {
				//New Object
				GameObject * obj = new GameObject();
				getline(file, line);
				sscanf(line.c_str(), "COMPONENT:");
				while (getline(file, line)) {
					if (!line.compare("")) break;
					else if (!line.compare("TRANSFORM")) {
						float x, y, z;
						getline(file, line);
						sscanf(line.c_str(), "pos: %f, %f, %f", &x, &y, &z);
						Vector3 position = Vector3(x, y, z);
						getline(file, line);
						sscanf(line.c_str(), "rot: %f, %f, %f", &x, &y, &z);
						Vector3 rotation = Vector3(x, y, z);
						getline(file, line);
						sscanf(line.c_str(), "scl: %f, %f, %f", &x, &y, &z);
						Vector3 scale = Vector3(x, y, z);
						obj->updateTransform(position, rotation, scale);
					}
					else if (!line.compare("SPRITE")) {
						getline(file, line);
						SpriteRenderer * sprRender = new SpriteRenderer(strdup(line.c_str()));
						obj->AddComponent(sprRender);
					}
					else if (!line.compare("ANIMATION")) {

					}
				}
				objectList.push_back(obj);
			}
		}
		file.close();
	}

	else cout << "Unable to open file";
}

void SceneManager::PrintAll()
{
	for (std::vector<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		(*it)->Print();
		cout << "\n";
	}
}

void SceneManager::DrawAll()
{
	if (usedCamera == NULL) { cout << "No camera selected.\n"; return; }

	for (std::vector<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->isActive)
			(*it)->Draw();
	}
}

void SceneManager::DeleteAll()
{
	for (std::vector<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		delete *it;
	}
	objectList.clear();

	for (std::vector<Camera*>::iterator it = cameraList.begin(); it != cameraList.end(); ++it) {
		delete *it;
	}
	cameraList.clear();

	usedCamera = NULL;
	delete shaders;
	free(sceneName);

	ResourceManager::GetInstance()->DeleteAll();
}

void SceneManager::UseCamera(int camNum)
{
	usedCamera = cameraList.at(camNum);
}

void SceneManager::Update(float deltaTime)
{
	time += deltaTime;

	for (std::vector<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->isActive)
			(*it)->Update();
	}
}
