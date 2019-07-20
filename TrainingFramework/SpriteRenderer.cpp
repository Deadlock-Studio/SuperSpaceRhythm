#include "stdafx.h"
#include "Transform.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
	type = SPRITE;
}

SpriteRenderer::SpriteRenderer(char * filename) : SpriteRenderer()
{
	ResourceManager * resource = ResourceManager::GetInstance();

	//generate sprite id in resource manager
	textureId = resource->LoadTGA(filename);

	//set scale for sprite
	Texture * text = resource->GetTexture(textureId);

	scaleMatrix.SetScale(text->width/pixelsPerUnit, text->height/pixelsPerUnit, 1);
}


SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Draw()
{
	if (textureId == -1) return;

	ResourceManager * resource = ResourceManager::GetInstance();
	Camera * cam = SceneManager::GetInstance()->usedCamera;
	Transform * objTrans = ((GameObject*)parentObj)->GetComponent<Transform>();

	Matrix mvp = objTrans->getModelMatrix() * scaleMatrix * cam->m_view * cam->m_perspective;
	
	Model * model = resource->GetSquare();
	Texture * text = resource->GetTexture(textureId);
	Shaders * shaders = SceneManager::GetInstance()->shaders;

	glUseProgram(shaders->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	glEnableVertexAttribArray(shaders->positionLoc);
	glVertexAttribPointer(shaders->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	glEnableVertexAttribArray(shaders->uvLoc);
	glVertexAttribPointer(shaders->uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 1));

	glUniformMatrix4fv(shaders->mvpLoc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);

	glUniform1i(shaders->textureLoc, 0);

	glDrawElements(GL_TRIANGLES, model->nIndices, GL_UNSIGNED_INT, 0);
}
