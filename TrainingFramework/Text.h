#pragma once
#include "Font.h"
class Text
{
public:
	Text();
	Text(Font* font, char* context, Vector4 color, float posX, float posY, float scaleX, float scaleY);
	Text(Font* font, char* context, Vector4 color, float posX, float posY, float scaleX, float scaleY, float TTL);
	~Text();

	void Draw() {
		font->DrawString(context, color, posX, posY, scaleX, scaleY);
	}
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void FadeOut(float fadeSpeed);
	void FadeIn(float fadeSpeed);
	void FadeOff();
	void UpdatePosition(float x, float y);

	bool isActive = true;
	bool dying = false;
	float TTL = 0;
	float fadeOut = 0;
	float fadeIn = 0;
	
	Font*	font;
	char*	context;
	float	posX, posY;
	float	scaleX, scaleY;
	Vector4	color;
};

