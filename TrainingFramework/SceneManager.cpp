#include "stdafx.h"
#include "Control.h"
#include"Animation.h"
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
				//New Instance
				ObjectInstance* tmp = new ObjectInstance;
				char* name = new char[300];
				getline(file, line);
				sscanf(line.c_str(), "NAME %s", name);
				tmp->name = name;

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
						float spriteX, spriteY, spriteW, spriteH, textureW, textureH;
						getline(file, line);
						sscanf(line.c_str(), "anim: %f, %f, %f, %f, %f, %f", &spriteX, &spriteY, &spriteW, &spriteH, &textureW, &textureH);
						getline(file, line);
						Animation * anim = new Animation(strdup(line.c_str()), spriteX, spriteY, spriteW, spriteH, textureW, textureH);
						obj->AddComponent(anim);
					}
					else if (!line.compare("CONTROL")) {
						Control * control = new Control();
						obj->AddComponent(control);
					}
				}
				tmp->blueprint = obj;
				objectList.push_back(tmp);
			}
		}
		file.close();
	}

	else cout << "Unable to open file";
}

void SceneManager::PrintAll()
{
	for (std::vector<ObjectInstance*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		(*it)->blueprint->Print();
		cout << "\n";
	}
}

void SceneManager::DrawAll()
{
	if (usedCamera == NULL) { cout << "No camera selected.\n"; return; }

	for (std::vector<ObjectInstance*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->InstanceList.size() != 0)
		{
			for (std::vector<GameObject*>::iterator it1 = ((*it)->InstanceList).begin(); it1 != ((*it)->InstanceList).end(); ++it1)
			{
				if ((*it1)->isActive)
				{
					(*it1)->Draw();
				}
			}
		}
	}
}

void SceneManager::DeleteAll()
{
	for (std::vector<ObjectInstance*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		delete[] (*it)->name;
		delete (*it)->blueprint;
		for (std::vector<GameObject*>::iterator it1 = ((*it)->InstanceList).begin(); it1 != ((*it)->InstanceList).end(); ++it1)
		{
			delete (*it1);
		}
		delete (*it);
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

void SceneManager::SpawnObject(ObjectInstance* ins, float x, float y, float z) {
	GameObject* obj = new GameObject(); 
	Vector3 pos; pos.x = x; pos.y = y; pos.z = z;
	obj->updateTransform(pos, ins->blueprint->transform->rotation, ins->blueprint->transform->scale);
	//TODO: Make component check for efficiency//////
	if (ins->blueprint->GetComponent<SpriteRenderer>() != NULL)
	{
		SpriteRenderer * sprRender = new 
			SpriteRenderer(ResourceManager::GetInstance()
				->GetTexture(ins
					->blueprint->GetComponent<SpriteRenderer>()->textureId)->filename);
		obj->AddComponent(sprRender);
	}
	if (ins->blueprint->GetComponent<Control>() != NULL)
	{
		Control * control = new Control();
		obj->AddComponent(control);
	}
	if (ins->blueprint->GetComponent<Animation>() != NULL)
	{
		Animation * anim = new Animation(ResourceManager::GetInstance()->GetTexture(ins->blueprint->GetComponent<Animation>()->textureId)->filename, 
			ins->blueprint->GetComponent<Animation>()->spriteX, ins->blueprint->GetComponent<Animation>()->spriteY, 
			ins->blueprint->GetComponent<Animation>()->spriteW, ins->blueprint->GetComponent<Animation>()->spriteH, 
			ins->blueprint->GetComponent<Animation>()->textureW, ins->blueprint->GetComponent<Animation>()->textureH);
		obj->AddComponent(anim);
	}
	///////////////////////////////////////////////
	ins->InstanceList.push_back(obj);
}

SceneManager::ObjectInstance* SceneManager::getObjectInstance(char* name) {
	for (std::vector<ObjectInstance*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if (strcmp((*it)->name, name) == 0) return (*it);
	}
	return NULL;
};

void SceneManager::Update(float deltaTime)
{
	time += deltaTime;

	for (std::vector<ObjectInstance*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		if ((*it)->InstanceList.size() != 0)
		{
			for (std::vector<GameObject*>::iterator it1 = ((*it)->InstanceList).begin(); it1 != ((*it)->InstanceList).end(); ++it1)
			{
				if ((*it1)->isActive)
				{
					(*it1)->Update(deltaTime);
				}
			}
		}
	}
}
