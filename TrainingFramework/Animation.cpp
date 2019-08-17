#include "stdafx.h"
#include "Animation.h"
#include "Transform.h"
#define NUM_VERTICES 4
#define NUM_INDICES 6

Animation::Animation()
{
}

Animation::Animation(char * filename, float spriteX, float spriteY, float spriteW, float spriteH, int nSprite, float animSpeed, int pixelsPerUnit) : Animation()
{
	ResourceManager * resource = ResourceManager::GetInstance();

	//generate sprite id in resource manager
	textureId = resource->LoadTGA(filename);
	Texture * text = resource->GetTexture(textureId);

	// set scale for sprite
	this->pixelsPerUnit = pixelsPerUnit;
	scaleMatrix.SetScale((float)spriteW/pixelsPerUnit * GRID_SIZE, (float)spriteH / pixelsPerUnit * GRID_SIZE, 1);

	//Initialize u,v
	this->spriteX = spriteX;
	this->spriteY = spriteY;
	this->spriteW = spriteW;
	this->spriteH = spriteH;
	this->nSprite = nSprite;
	this->animSpeed = animSpeed;

	float a_W = spriteW;
	float a_H = spriteH;
	float a_u = (float)spriteW / (float)text->width;
	float a_v = (float)spriteH / (float)text->height;

	Vertex verticesList[4];

	verticesList[0].pos = Vector3(-0.5, -0.5, 0);
	verticesList[1].pos = Vector3(-0.5, 0.5, 0);
	verticesList[2].pos = Vector3(0.5, 0.5, 0);
	verticesList[3].pos = Vector3(0.5, -0.5, 0);

	verticesList[0].uv = Vector2((float)spriteX / text->width, (float)spriteY / text->height);
	verticesList[1].uv = Vector2((float)spriteX / text->width, (float)(spriteY + spriteH) / text->height);
	verticesList[2].uv = Vector2((float)(spriteX + spriteW) / text->width, (float)(spriteY + spriteH) / text->height);
	verticesList[3].uv = Vector2((float)(spriteX + spriteW) / text->width, (float)spriteY / text->height);

	GLuint temp;
	for (int i = 0; i < nSprite; i++) {
		glGenBuffers(1, &temp);
		glBindBuffer(GL_ARRAY_BUFFER, temp);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* NUM_VERTICES, verticesList, GL_STATIC_DRAW);
		vboId.push_back(temp);

		for (int j = 0; j < 4; j++) {
			verticesList[j].uv.x += a_u;
		}
	}
}

//TODO Properly unbind GPU
Animation::~Animation()
{
}

void Animation::Draw()
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
	glBindBuffer(GL_ARRAY_BUFFER, vboId.at(activeSprite));
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

void Animation::Update(float deltaTime)
{
	RunAnimation(deltaTime);
}

Component * Animation::Clone()
{
	Texture * text = ResourceManager::GetInstance()->GetTexture(textureId);
	Animation * anime = new Animation(text->filename, spriteX, spriteY, spriteW, spriteH, nSprite, animSpeed, pixelsPerUnit);
	return anime;
}

void Animation::RunAnimation(float deltaTime)
{	
	//DWORD start_t = GetTickCount();
	//DWORD end_t = GetTickCount();
	tick += 1.0f/FPS;
	if (tick >= animSpeed * 1.0f/FPS) {
		tick = 0;
		activeSprite++;
		if (activeSprite == nSprite)
			activeSprite = 0;
	}
}
