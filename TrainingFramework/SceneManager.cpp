#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include "HandyString.h"

using namespace std;

SceneManager * SceneManager::ms_pInstance = NULL;

void SceneManager::LoadScene(char * filename)
{
	sceneName = strdup(HandyString::getFileName(string(filename)).c_str());
	ResourceManager * resource = ResourceManager::GetInstance();

	string line;
	ifstream file(filename);
	if (file.is_open())
	{
		while (getline(file, line)) {
			int id;
			if (line.compare("") == 0) break;
			if (sscanf(line.c_str(), "OBJECT %d\n", &id)) {
				//New object, read until EoL
				Object * obj = new Object();

				float x, y, z;
				getline(file, line);
				sscanf(line.c_str(), "POS: %f, %f, %f\n", &x, &y, &z);
				obj->setPosition(x, y, z);
				getline(file, line);
				sscanf(line.c_str(), "ROT: %f, %f, %f\n", &x, &y, &z);
				obj->setRotation(x, y, z);
				getline(file, line);
				sscanf(line.c_str(), "SCA: %f, %f, %f\n", &x, &y, &z);
				obj->setScale(x, y, z);

				getline(file, line);
				sscanf(line.c_str(), "EXT:\n");

				int modelCount = 0, verShaderCount = 0, fragShaderCount = 0, heightmapCount = 0;
				char * verFile,  * fragFile;

				while (getline(file, line)) {
					if (line.compare("") == 0) break;
					if (!HandyString::getFileExt(line).compare("nfg")) {
						//Model
						if (++modelCount > 1) { cout << "An Object can only have one model\n"; delete obj; return; }
						obj->modelID = resource->LoadNFG(strdup(line.c_str()));
					}
					else if (!HandyString::getFileExt(line).compare("tga")) {
						if (!HandyString::getTypeFolder(line).compare("Textures")) {
							//Texture
							bool exist = false;
							int text = resource->LoadTGA(strdup(line.c_str()));
							for (std::vector<int>::iterator it = obj->textureID.begin(); it != obj->textureID.end(); ++it) {
								if (*(it) == text) exist = true;
							} 
							if (!exist) obj->textureID.push_back(text);
						}	
						else if (!HandyString::getTypeFolder(line).compare("CubeTextures")) {
							//CubeTexture
							bool exist = false;

							int cube = resource->LoadCubeTGA(strdup(line.c_str()));
							for (std::vector<int>::iterator it = obj->cubeTextureID.begin(); it != obj->cubeTextureID.end(); ++it) {
								if (*(it) == cube) exist = true;
							}
							if (!exist) obj->cubeTextureID.push_back(cube);
						}
					}
					else if (!HandyString::getFileExt(line).compare("raw")) {
						//Heightmap
						if (++heightmapCount > 1) { cout << "An Object can only have one height map\n"; delete obj; return; }
						resource->LoadRaw(strdup(line.c_str()));
					}
					else if (!HandyString::getFileExt(line).compare("vs")) {
						//Vertex Shader
						if (++verShaderCount > 1) { cout << "An Object can only have one vertex shader\n"; delete obj; return; }
						verFile = strdup(line.c_str());
					}
					else if (!HandyString::getFileExt(line).compare("fs")) {
						//Fragment Shader
						if (++fragShaderCount > 1) { cout << "An Object can only have one fragment shader\n"; delete obj; return; }
						fragFile = strdup(line.c_str());
					}
				}

				if (verShaderCount == 1 && fragShaderCount == 1)
					obj->shadersID = resource->LoadShaders(verFile, fragFile);
				obj->initMatrix();
				objectList.push_back(obj);
			}
			else if (sscanf(line.c_str(), "CAMERA %d\n", &id)) {
				//New Camera
				Camera * cam = new Camera();

				float x;
				getline(file, line);
				sscanf(line.c_str(), "NEAR: %f\n", &x);
				cam->nearPlane = x;
				getline(file, line);
				sscanf(line.c_str(), "FAR: %f\n", &x);
				cam->farPlane = x;
				getline(file, line);
				sscanf(line.c_str(), "FOV: %f\n", &x);
				cam->fov = x;
				getline(file, line);
				sscanf(line.c_str(), "SPEED: %f\n", &x);
				cam->speed = x;
				getline(file, line);
				sscanf(line.c_str(), "ROTSPEED: %f\n", &x);
				cam->rotSpeed = x;

				cam->InitCamera();
				cameraList.push_back(cam);
			}
		}
		file.close();
	}

	else cout << "Unable to open file";
}

void SceneManager::PrintAll()
{
	for (std::vector<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		(*it)->Print();
	}
}

void SceneManager::DrawAll()
{
	if (usedCamera == NULL) { cout << "No camera selected.\n"; }

	for (std::vector<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		(*it)->Draw(usedCamera);
	}
}

void SceneManager::DeleteAll()
{
	for (std::vector<Object*>::iterator it = objectList.begin(); it != objectList.end(); ++it) {
		delete *it;
	}

	for (std::vector<Camera*>::iterator it = cameraList.begin(); it != cameraList.end(); ++it) {
		delete *it;
	}

	usedCamera = NULL;
}

void SceneManager::UseCamera(int camNum)
{
	usedCamera = cameraList.at(camNum);
}

void SceneManager::Update(float deltaTime)
{
	time += deltaTime;
	if (deltaTime > 3000)
		deltaTime -= 3000;
}
