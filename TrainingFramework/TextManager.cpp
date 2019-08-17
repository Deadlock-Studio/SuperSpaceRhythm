#include "stdafx.h"
#include "TextManager.h"


TextManager * TextManager::ms_pInstance = NULL;

void TextManager::LoadFont(char* fileName)
{
	string line;
	ifstream file(fileName);
	if (file.is_open())
	{
		while (getline(file, line)) {
			fontList.push_back(new Font(_strdup(line.c_str())));
		}
		file.close();
	}
}

Font* TextManager::GetFontByName(char* name)
{
	for (list<Font*>::iterator it = fontList.begin(); it != fontList.end(); ++it) {
		if (!strcmp((*it)->name, name)) return (*it);
	}
	return NULL;
}

void TextManager::Update(float deltaTime)
{
	for (std::list<Text*>::iterator it = textList.begin(); it != textList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->Update(deltaTime);
		}
	}
}

void TextManager::LateUpdate(float deltaTime)
{
	for (std::list<Text*>::iterator it = textList.begin(); it != textList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->LateUpdate(deltaTime);
		}
	}

	FlushRemoval();
}

void TextManager::Draw()
{
	if (SceneManager::GetInstance()->usedCamera == NULL) { cout << "No camera selected.\n"; return; }

	for (std::list<Text*>::iterator it = textList.begin(); it != textList.end(); ++it) {
		if ((*it)->isActive) {
			(*it)->Draw();
		}
	}
}

void TextManager::UnloadAll()
{
	for (std::list<Font*>::iterator it = fontList.begin(); it != fontList.end(); ++it) {
		delete *it;
	}
	fontList.clear();

	for (std::list<Text*>::iterator it = textList.begin(); it != textList.end(); ++it) {
		delete *it;
	}
	textList.clear();
}

Text* TextManager::AddText(char * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY)
{
	Font* found = GetFontByName(font);
	if (found == NULL) { cout << "There is no such font" << endl; return NULL; }

	Text* text = new Text(found, context, color, posX, posY, scaleX, scaleY);
	textList.push_back(text);
	return text;
}

Text * TextManager::AddText(char * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY, float TTL)
{
	Font* found = GetFontByName(font);
	if (found == NULL) { cout << "There is no such font" << endl; return NULL; }

	Text* text = new Text(found, context, color, posX, posY, scaleX, scaleY, TTL);
	textList.push_back(text);
	return text;
}

void TextManager::DeleteText(Text * key)
{
	delete key;
	textList.remove(key);
}

void TextManager::AddToRemoval(Text * key)
{
	removalList.push_back(key);
}

void TextManager::FlushRemoval()
{
	for (std::list<Text*>::iterator it = removalList.begin(); it != removalList.end(); ++it) {
		delete (*it);
		textList.remove(*it);
	}
	removalList.clear();
}
