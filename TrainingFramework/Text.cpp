#include "stdafx.h"
#include "Text.h"


Text::Text()
{
}

Text::Text(Font * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY)
{
	this->font = font;
	this->context = _strdup(context);
	this->color = color;
	this->posX = posX;
	this->posY = posY;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

Text::Text(Font * font, char * context, Vector4 color, float posX, float posY, float scaleX, float scaleY, float TTL) : 
	Text(font, context, color, posX, posY, scaleX, scaleY)
{
	this->TTL = TTL;
	this->dying = true;
}


Text::~Text()
{
}

void Text::Update(float deltaTime)
{
}

void Text::LateUpdate(float deltaTime)
{
	if (fadeOut) {
		if (color.w > 0) {
			color.w -= fadeOut * deltaTime;
			if (color.w < 0)
				color.w = 0;
		}	
	}

	if (fadeIn) {
		if (color.w < 1) {
			color.w += fadeIn * deltaTime;
			if (color.w > 1)
				color.w = 1;
		}
	}

	if (dying)
		if (TTL > 0) {
			TTL -= deltaTime;
			if (TTL <= 0)
				TextManager::GetInstance()->AddToRemoval(this);
		}
}

void Text::FadeOut(float fadeSpeed)
{
	fadeIn = 0;
	fadeOut = fadeSpeed;
}

void Text::FadeIn(float fadeSpeed)
{
	fadeOut = 0;
	fadeIn = fadeSpeed;
}

void Text::FadeOff()
{
	fadeIn = 0;
	fadeOut = 0;
}
