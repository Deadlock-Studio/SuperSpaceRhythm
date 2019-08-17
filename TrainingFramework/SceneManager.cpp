#include "stdafx.h"
#include "Control.h"
#include "Animation.h"
#include "Player.h"
#include "Boss.h"
#include "HealthPotion.h"
#include "Collision2D.h"
#include "Bullet.h"
#include "Room.h"
#include "Button.h"
using namespace std;

SceneManager * SceneManager::ms_pInstance = NULL;

void SceneManager::Init()
{
	shaders = new Shaders("../Resources/Shaders/CommonShaderVS.vs",
		"../Resources/Shaders/CommonShaderFS.fs");

	fontShaders = new Shaders("../Resources/Shaders/FontShaderVS.vs",
		"../Resources/Shaders/FontShaderFS.fs");

	SceneManager::GetInstance()->LoadScene("../Resources/Templates/Template1.txt");
	SceneManager::GetInstance()->LoadScene("../Resources/Templates/GUI.txt");
	SceneManager::GetInstance()->LoadScene("../Resources/Templates/Mob.txt");

	SceneManager::GetInstance()->UseCamera(0);
}

void SceneManager::LoadScene(char * filename)
{
	sceneName = _strdup(HandyString::getFileName(string(filename)).c_str());

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
				cameraList.push_back(GetCam(file));
			}
			else if (sscanf(line.c_str(), "BLUEPRINT %d", &id)) {
				Blueprint * blueprint = new Blueprint();

				//Get Object name
				blueprint->name = GetName(file);

				GetComponent(file, blueprint);

				blueprintList.push_back(blueprint);
			}
		}
		file.close();
	}

	else cout << "Unable to open file";
}

void SceneManager::GetComponent(ifstream & file, Blueprint * blueprint)
{
	string line;
	getline(file, line);
	sscanf(line.c_str(), "COMPONENT:");
	while (getline(file, line)) {
		if (!line.compare("")) break;
		else if (!line.compare("SPRITE")) {
			blueprint->AddComponent(GetSprite(file));
		}
		else if (!line.compare("ANIMATION")) {
			blueprint->AddComponent(GetAnim(file));
		}
		else if (!line.compare("CONTROL")) {
			blueprint->AddComponent(new Control());
		}
		else if (!line.compare("COLLISION")) {
			blueprint->AddComponent(GetCollision(file));
		}
		else if (!line.compare("HP")) {
			blueprint->AddComponent(GetHP(file));
		}
		else if (!line.compare("BUTTON")) {
			blueprint->AddComponent(new Button());
		}
	}
}

Camera * SceneManager::GetCam(ifstream & file)
{
	string line;
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
	return cam;
}

char * SceneManager::GetName(ifstream & file)
{
	string line;
	char name[300];
	getline(file, line);
	sscanf(line.c_str(), "NAME: %s", name);
	return _strdup(name);
}

void SceneManager::GetTransform(ifstream & file, GameObject * obj)
{
	string line;
	getline(file, line);
	sscanf(line.c_str(), "TRANSFORM:");
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
	obj->UpdateTransform(position, rotation, scale);
}

SpriteRenderer * SceneManager::GetSprite(ifstream & file)
{
	string line;
	int pixelsPerUnit;
	getline(file, line);
	sscanf(line.c_str(), "pixelsPerUnit: %d", &pixelsPerUnit);
	getline(file, line);
	return new SpriteRenderer(_strdup(line.c_str()), pixelsPerUnit);
}

Animation * SceneManager::GetAnim(ifstream & file)
{
	string line;
	float spriteX, spriteY, spriteW, spriteH, nSprite, animSpeed;
	int pixelsPerUnit;
	getline(file, line);
	sscanf(line.c_str(), "pixelsPerUnit: %d", &pixelsPerUnit);
	getline(file, line);
	sscanf(line.c_str(), "anim: %f, %f, %f, %f, %f", &spriteX, &spriteY, &spriteW, &spriteH, &nSprite);
	getline(file, line);
	sscanf(line.c_str(), "anim_spd: %f", &animSpeed);
	getline(file, line);
	return new Animation(_strdup(line.c_str()), spriteX, spriteY, spriteW, spriteH, (int)nSprite, animSpeed, pixelsPerUnit);
}

Collision2D * SceneManager::GetCollision(ifstream & file)
{
	string line;
	float colW, colH, mass, res;
	char colType[10];
	getline(file, line);
	sscanf(line.c_str(), "body: %f, %f, %f, %f",&colW, &colH, &mass, &res);
	getline(file, line);
	sscanf(line.c_str(), "body_type: %s", &colType);
	return new Collision2D(colW, colH, mass,res, colType);
}

HP * SceneManager::GetHP(ifstream & file)
{
	string line;
	float health;
	getline(file, line);
	sscanf(line.c_str(), "health: %f", &health);
	return new HP(health);
}

void SceneManager::Draw()
{
	if (usedCamera == NULL) { cout << "No camera selected.\n"; return; }
	
	for (std::list<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->Draw();
		}
	}
}

void SceneManager::UnloadAll()
{
	UnloadObjects();

	for (std::vector<Blueprint*>::iterator it = blueprintList.begin(); it != blueprintList.end(); ++it)
		delete *it;
	blueprintList.clear();

	for (std::vector<Camera*>::iterator it = cameraList.begin(); it != cameraList.end(); ++it)
		delete *it;
	cameraList.clear();

	usedCamera = NULL;
	delete shaders;
	delete fontShaders;
	free(sceneName);
}

void SceneManager::UnloadObjects()
{
	for (std::list<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
		delete *it;
	objectList.clear();
}

void SceneManager::UseCamera(int camNum)
{
	usedCamera = cameraList.at(camNum);
}

GameObject * SceneManager::SpawnObject(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation)
{
	if (blueprint == NULL) {
		cout << "Can not spawn null object" << endl;
		return NULL;
	}

	GameObject* obj = new GameObject(blueprint, pos, scale, rotation);
	objectList.push_back(obj);

	return obj;
}

GUI * SceneManager::SpawnGUI(Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation)
{
	if (blueprint == NULL) {
		cout << "Can not spawn null object" << endl;
		return NULL;
	}

	GUI* gui = new GUI(blueprint, pos, scale, rotation);
	objectList.push_back(gui);

	return gui;
}

GameObject * SceneManager::SpawnBullet(float x, float y, float mX, float mY, char* type)
{
	GameObject * pBullet = nullptr;
		pBullet = new Bullet(x, y, mX, mY, type);
		objectList.push_back(pBullet);
	return pBullet;
}

Blueprint* SceneManager::GetBlueprintByName(char* name) {
	for (std::vector<Blueprint*>::iterator it = blueprintList.begin(); it != blueprintList.end(); ++it) {
		if (!strcmp((*it)->name, name)) return (*it);
	}
	return NULL;
}

GameObject * SceneManager::GetObjectByName(char * name)
{
	for (std::list<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if (!strcmp((*it)->name, name)) return (*it);
	}
	return NULL;
}

void SceneManager::Update(float deltaTime)
{
	for (std::list<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->Update(deltaTime);
		}
	}
}

void SceneManager::LateUpdate(float deltaTime)
{
	for (std::list<GameObject*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->LateUpdate(deltaTime);
		}
	}
}

void SceneManager::safeDel_k(GameObject* key) {
	delete key;
	SceneManager::GetInstance()->objectList.remove(key);
}


void SceneManager::addToRemovalList(GameObject * key)
{
	removalList.push_back(key);
}

void SceneManager::deleteRemovalList()
{
	//crucial, avoid remove same obj twice if collide 2 obj at the same time
	removalList.sort();
	removalList.unique();
	for (std::list<GameObject*>::iterator it = removalList.begin(); it != removalList.end(); ++it) {
		safeDel_k(*it);
	}
	removalList.clear();
}
