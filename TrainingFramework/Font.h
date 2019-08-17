#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H
#include <map>
using namespace std;

struct Character {
	GLuint		TextureID;  // ID handle of the glyph texture
	Matrix		scaleMatrix;// Scale a square to get resulting model
	Vector2		Size;       // Size of glyph
	Vector2		Bearing;    // Offset from baseline to left/top of glyph
	GLuint		Advance;    // Offset to advance to next glyph
};

class Font
{
public:
	Font();
	Font(char* fontName);
	~Font();

	char* name;
	FT_Library m_ft;
	FT_Face m_face;
	map<GLubyte, Character> Characters;
	void DrawString(const char *text, Vector4 color, float x, float y, float scaleX, float scaleY);
};

