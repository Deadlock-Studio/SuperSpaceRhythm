#include "stdafx.h"
#include "Object.h"
#include <iostream>
#include "ResourceManager.h"
#include "SceneManager.h"

Object::Object()
{
}


Object::~Object()
{
}

void Object::Print()
{
	cout << "Model: " << modelID << '\n';
	cout << "Texture: \n";
	if (!textureID.empty())
		for (std::vector<int>::iterator it = textureID.begin(); it != textureID.end(); ++it) {
			cout << ResourceManager::GetInstance()->GetTexture(*it)->filename << '\n';
		}
	cout << "Shader program: " << shadersID << '\n';

	cout << "Pos: " << position.x << ' ' << position.y << ' ' << position.z << '\n';
	cout << "Rot: " << rotation.x << ' ' << rotation.y << ' ' << rotation.z << '\n';
	cout << "Scale: " << scale.x << ' ' << scale.y << ' ' << scale.z << '\n';
	cout << "\n";
}

void Object::Draw(Camera * camera)
{
	Model * model = ResourceManager::GetInstance()->GetModel(modelID);
	Shaders * shaders = ResourceManager::GetInstance()->GetShaders(shadersID);
	
	Matrix mvp = m_model * camera->m_view * camera->m_perspective;

	glUseProgram(shaders->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	glEnableVertexAttribArray(shaders->positionLoc);
	glVertexAttribPointer(shaders->positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(shaders->uvLoc);
	glVertexAttribPointer(shaders->uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) * 4));

	glUniformMatrix4fv(shaders->mvpLoc, 1, GL_FALSE, (GLfloat*)&mvp.m[0][0]);
	glUniformMatrix4fv(shaders->m_model, 1, GL_FALSE, (GLfloat*)&m_model.m[0][0]);

	glUniform3f(shaders->u_cam, camera->position.x, camera->position.y, camera->position.z);
	glUniform1f(shaders->u_fogStart, 9.0f); //fog start
	glUniform1f(shaders->u_fogRange, 10.0f); //fog range
	glUniform3f(shaders->u_fogColor, 1, 1, 1); //fog Color

	if (!textureID.empty()) {
		for (int i = 0; i < textureID.size(); i++) {
			Texture * texture = ResourceManager::GetInstance()->GetTexture(textureID.at(i));
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, texture->texId);
		}
		if (shaders->u_blendmap != -1) {
			glUniform1i(shaders->u_blendmap, 0);
			glUniform1i(shaders->u_rock, 1);
			glUniform1i(shaders->u_dirt, 2);
			glUniform1i(shaders->u_grass, 3);
		}
		else if (shaders->u_dismap != -1) {
			glUniform1f(shaders->u_time, SceneManager::GetInstance()->time);
			glUniform1i(shaders->textureLoc, 0);
			glUniform1i(shaders->u_dismap, 1);
			glUniform1i(shaders->u_mask, 2);
		}
		else glUniform1i(shaders->textureLoc, 0);
	}

	if (!cubeTextureID.empty()) {
		CubeTexture * texture = ResourceManager::GetInstance()->GetCubeTexture(cubeTextureID.at(0));
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->texId);
	}

	glDrawElements(GL_TRIANGLES, model->nIndices, GL_UNSIGNED_INT, 0);
}

void Object::initMatrix()
{
	Matrix _trans;
	Matrix _scale;
	Matrix _rotate[4];

	_trans.SetTranslation(position.x, position.y, position.z);
	_scale.SetScale(scale.x, scale.y, scale.z);
	_rotate[0].SetRotationX(rotation.x);
	_rotate[1].SetRotationY(rotation.y);
	_rotate[2].SetRotationZ(rotation.z);
	_rotate[3] = _rotate[0] * _rotate[1] * _rotate[2];

	m_model =  _scale * _rotate[3] * _trans;
}
