#include "stdafx.h"
#include "Transform.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::SpriteRenderer(char * filename, int pixelPerUnit)
{
	this->pixelsPerUnit = pixelPerUnit;

	ResourceManager * resource = ResourceManager::GetInstance();

	//generate sprite id in resource manager
	textureId = resource->LoadTGA(filename);

	//set scale for sprite
	Texture * text = resource->GetTexture(textureId);

	scaleMatrix.SetScale((float)text->width / pixelsPerUnit * GRID_SIZE, (float)text->height / pixelsPerUnit  * GRID_SIZE, 1);
}


SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Draw()
{
	if (textureId == -1) return;

	ResourceManager * resource = ResourceManager::GetInstance();
	Camera * cam = SceneManager::GetInstance()->usedCamera;
	Transform * objTrans = ((GameObject*)parentObj)->transform;

	Matrix mvp = scaleMatrix * objTrans->m_model * cam->m_view * cam->m_perspective;
	
	Model * model = resource->GetSquare();
	Texture * text = resource->GetTexture(textureId);
	Shaders * shaders = SceneManager::GetInstance()->shaders;

	glUseProgram(shaders->program);

	//Model, uv
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	glEnableVertexAttribArray(shaders->positionLoc);
	glVertexAttribPointer(shaders->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	glEnableVertexAttribArray(shaders->uvLoc);
	glVertexAttribPointer(shaders->uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 1));

	//MvP matrix
	glUniformMatrix4fv(shaders->mvpLoc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);
	glUniform1f(shaders->alphaLoc, alpha);

	//Texture
	glBindTexture(GL_TEXTURE_2D, text->texId);

	glUniform1i(shaders->textureLoc, 0);

	glDrawElements(GL_TRIANGLES, model->nIndices, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::LateUpdate(float deltaTime)
{
	if (fadeOut) {
		if (alpha > 0) {
			alpha -= fadeOut * deltaTime;
			if (alpha < 0)
				alpha = 0;
		}
	}

	if (fadeIn) {
		if (alpha < 1) {
			alpha += fadeIn * deltaTime;
			if (alpha > 1)
				alpha = 1;
		}
	}
}

Component * SpriteRenderer::Clone()
{
	SpriteRenderer * sprite = new SpriteRenderer();
	sprite->scaleMatrix = scaleMatrix;
	sprite->textureId = textureId;
	sprite->pixelsPerUnit = pixelsPerUnit;
	return sprite;
}

void SpriteRenderer::FadeOff()
{
	fadeOut = 0;
	fadeIn = 0;
}

void SpriteRenderer::FadeIn(float fadeSpeed)
{
	fadeOut = 0;
	fadeIn = fadeSpeed;
}

void SpriteRenderer::FadeOut(float fadeSpeed)
{
	fadeIn = 0;
	fadeOut = fadeSpeed;
}
