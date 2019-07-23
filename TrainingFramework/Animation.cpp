#include "stdafx.h"
#include "Animation.h"
#include "Transform.h"
#define NUM_VERTICES 4
#define NUM_INDICES 6
#define ANIM_SPEED 10

Animation::Animation()
{
	type = ANIMATION;
}

Animation::Animation(char * filename,float spriteX,float spriteY, float spriteW,float spriteH, float textureW,float textureH) : Animation()
{
	ResourceManager * resource = ResourceManager::GetInstance();
	this->spriteX = spriteX;
	this->spriteY = spriteY;
	this->spriteW = spriteW;
	this->spriteH = spriteH;
	this->textureW = textureW;
	this->textureH = textureH;
	//generate sprite id in resource manager
	textureId = resource->LoadTGA(filename);
	text = resource->GetTexture(textureId);

	//anim sprite model
	Vector2 origin = Vector2(spriteW / 2, spriteH / 2);
	Vector3 delta = Vector3(origin.x - (float)spriteW / 2, origin.y - (float)spriteH / 2, 0.0);

	verticesList[0].pos = Vector3(-(float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;
	verticesList[1].pos = Vector3(-(float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesList[2].pos = Vector3((float)spriteW / 2, (float)spriteH / 2, 0.0f) - delta;
	verticesList[3].pos = Vector3((float)spriteW / 2, -(float)spriteH / 2, 0.0f) - delta;

	verticesList[1].uv = Vector2((float)spriteX / textureW, (float)(spriteY + spriteH) / textureH);
	verticesList[0].uv = Vector2((float)spriteX / textureW, (float)spriteY / textureH);
	verticesList[3].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)spriteY / textureH);
	verticesList[2].uv = Vector2((float)(spriteX + spriteW) / textureW, (float)(spriteY + spriteH) / textureH);

	int indicesList[] = { 3, 2, 1, 0, 3, 1 };
	a_W = spriteW;
	a_H = spriteH;
	a_u = (float)spriteW /(float)textureW;
	a_v = (float)spriteH / (float)textureH;

	glGenBuffers(1, &a_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, a_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* NUM_VERTICES, verticesList, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &a_iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* NUM_INDICES, indicesList, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// set scale for sprite
	scaleMatrix.SetScale(text->width / pixelsPerUnit, text->height / pixelsPerUnit, 1);
}


Animation::~Animation()
{
	delete[] text;
}

void Animation::Draw()
{
	if (textureId == -1) return;

	ResourceManager * resource = ResourceManager::GetInstance();
	Camera * cam = SceneManager::GetInstance()->usedCamera;
	Transform * objTrans = ((GameObject*)parentObj)->transform;

	Matrix mvp = (objTrans->m_model * scaleMatrix) * cam->m_view * cam->m_perspective;

	Shaders * shaders = SceneManager::GetInstance()->shaders;

	glUseProgram(shaders->program);

	//Model, uv
	glBindBuffer(GL_ARRAY_BUFFER, a_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_iboId);

	glEnableVertexAttribArray(shaders->positionLoc);
	glVertexAttribPointer(shaders->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(shaders->uvLoc);
	glVertexAttribPointer(shaders->uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 1));

	//MvP matrix
	glUniformMatrix4fv(shaders->mvpLoc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);

	//Texture
	glBindTexture(GL_TEXTURE_2D, text->texId);
	glUniform1i(shaders->textureLoc, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Animation::Update(float deltaTime)
{
	RunAnimation(deltaTime);
}

void Animation::RunAnimation(float deltaTime)
{	
	tick++;
	if (tick >= ANIM_SPEED) {
		tick = 0;
		for (int i = 0; i < 4; i++) {
			verticesList[i].uv.x += a_u;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, a_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* NUM_VERTICES, verticesList, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
