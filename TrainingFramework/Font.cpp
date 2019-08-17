#include "stdafx.h"
#include "Font.h"

Font::Font()
{
}

Font::Font(char * fontName)
{
	name = _strdup(HandyString::getFileName(string(fontName)).c_str());

	if (FT_Init_FreeType(&m_ft))
	{
		printf("Could not init freetype library\n");
		return;
	}
	if (FT_New_Face(m_ft, fontName, 0, &m_face)) {
		printf("Could not open font %s\n", name);
		return;
	}
	FT_Set_Pixel_Sizes(m_face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate vbo scale matrix
		Matrix scaleMatrix;
		scaleMatrix.SetScale((float)m_face->glyph->bitmap.width, (float)m_face->glyph->bitmap.rows, 1);

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Now store character for later use
		Character character = {
			texture,
			scaleMatrix,
			Vector2((GLfloat)m_face->glyph->bitmap.width,(GLfloat)m_face->glyph->bitmap.rows),
			Vector2((GLfloat)m_face->glyph->bitmap_left, (GLfloat)m_face->glyph->bitmap_top),
			(GLuint)m_face->glyph->advance.x
		};
		Characters.insert(pair<GLubyte, Character>(c, character));
	}

	FT_Done_Face(m_face);
	FT_Done_FreeType(m_ft);
}

Font::~Font()
{
}

void Font::DrawString(const char * text, Vector4 color, float x, float y, float scaleX, float scaleY)
{
	ResourceManager * resource = ResourceManager::GetInstance();
	Camera * cam = SceneManager::GetInstance()->usedCamera;
	Model * model = resource->GetSquareTL();
	Shaders* fontShaders = SceneManager::GetInstance()->fontShaders;

	glUseProgram(fontShaders->program);
	glActiveTexture(GL_TEXTURE0);
	glUniform4f(fontShaders->colorLoc, color.x, color.y, color.z, color.w);

	//Model, uv
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	glEnableVertexAttribArray(fontShaders->positionLoc);
	glVertexAttribPointer(fontShaders->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(fontShaders->uvLoc);
	glVertexAttribPointer(fontShaders->uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 1));

	for (const char *p = text; *p; p++) {
		Character ch = Characters[*p];

		GLfloat xpos = x + ch.Bearing.x * scaleX;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scaleY;

		Matrix _trans;
		Matrix _scale;
		Matrix _rotation[4];

		_trans.SetTranslation(xpos, ypos, GUI_LAYER);
		_scale.SetScale(scaleX, scaleY, 1);
		_rotation[0].SetRotationX(0);
		_rotation[1].SetRotationY(0);
		_rotation[2].SetRotationZ(0);
		_rotation[3] = _rotation[0] * _rotation[1] * _rotation[2];
		Matrix mvp = ch.scaleMatrix * (_scale * _rotation[3] * _trans) * cam->m_view * cam->m_perspective;

		//MvP matrix
		glUniformMatrix4fv(fontShaders->mvpLoc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);

		//Texture
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glUniform1i(fontShaders->textureLoc, 0);

		glDrawElements(GL_TRIANGLES, model->nIndices, GL_UNSIGNED_INT, 0);

		x += (ch.Advance >> 6) * scaleX;
	}
}