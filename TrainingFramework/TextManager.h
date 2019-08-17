#pragma once
#include "Text.h"

class TextManager
{
public:
protected:
	TextManager() {}
	~TextManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	TextManager(const TextManager &) {}
	TextManager& operator =(const TextManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new TextManager;
	}
	static TextManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			GetInstance()->UnloadAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	void LoadFont(char* fileName);
	Font* GetFontByName(char* name);
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw();
	void UnloadAll();

	Text* AddText(char * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY);
	Text* AddText(char * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY, float TTL);
	void DeleteText(Text* key);
	void AddToRemoval(Text* key);
	void FlushRemoval();
protected:
	static TextManager * ms_pInstance;
	list<Text*> textList;
	list<Font*> fontList;
	list<Text*> removalList;
};

