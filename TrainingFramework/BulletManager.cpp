#include "stdafx.h"
#include "BulletManager.h"

int spiralDegree = 0;
int offset = -60;
int shotgunState = 0;
int reversespiralDegree = 0;
int arrowDegree = 0;
float perpendicular = cos(45 * PI / 180) * RADIUS;

BulletManager * BulletManager::ms_pInstance = NULL;

float xPixel(float degree, float posX, float radius) {
	return (float)(posX + radius * cos(degree * PI / 180));
}

float yPixel(float degree, float posY, float radius) {
	return (float)(posY + radius * sin(degree * PI / 180));
}

GameObject* addBullet(float x, float y, float xPixel, float yPixel, char* type) {
	GameObject* temp;
	temp = SceneManager::GetInstance()->SpawnBullet(x, y, xPixel, yPixel, type);
	return temp;
}

void BulletManager::Circle(Vector3 pos, char* type) {
	for (int i = 0; i < 360; i += 30)
	{
		addBullet(pos.x, pos.y, xPixel((float)i, pos.x, RADIUS), yPixel((float)i, pos.y, RADIUS), type);
	}
}

void BulletManager::Plus(Vector3 pos, char* type) {
	for (int i = 0; i < 360; i+=90)
	{
		addBullet(pos.x, pos.y, xPixel((float)i, pos.x, RADIUS), yPixel((float)i, pos.y, RADIUS), type);
	}
}

void BulletManager::Cross(Vector3 pos, char* type) {
	for (int i = 30; i < 360; i += 90) {
		addBullet(pos.x, pos.y, xPixel((float)i, pos.x, RADIUS), yPixel((float)i, pos.y, RADIUS), type);
	}

}

Vector2 Sys2EqSolve(float a, float b, float e, float c, float d, float f) {
	//solve system of 2 linear equations
	//ax + by = e
	//cx + dy = f
	Vector2 root;
	float det = a*d - b*c;
	root.x = (e*d - b*f) / det;
	root.y = (a*f - e*c) / det;
	return root;
}

Vector2 PerpendicularLine(float x, float y, float a, float b) {
	//evaluate the function of a line goes through (x,y) and perpendicular to y = ax + b
	Vector2 root;
	root.x = -1 / a;
	root.y = y - root.x * x;
	return root;
}

void BulletManager::Flower(Vector3 pos, char* type) {
	for (int i = 0; i < 2; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 90, pos.x, RADIUS),
			yPixel(spiralDegree + i * 90, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 90, pos.x, RADIUS),
			yPixel(spiralDegree + 180 + i * 90, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + i * 90, pos.x, RADIUS),
			yPixel(reversespiralDegree + i * 90, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + 180 + i * 90, pos.x, RADIUS),
			yPixel(reversespiralDegree + 180 + i * 90, pos.y, RADIUS), type);
	}
	reversespiralDegree -= 10;
	spiralDegree += 10;
	if (spiralDegree == 360) spiralDegree = 0;
	if (reversespiralDegree == -360) spiralDegree = 0;
}
void BulletManager::DoubleSpiral(Vector3 pos, char* type) {
	for (int i = 0; i < 1; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + 180 + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + i * 15, pos.x, RADIUS),
			yPixel(reversespiralDegree + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + 180 + i * 15, pos.x, RADIUS),
			yPixel(reversespiralDegree + 180 + i * 15, pos.y, RADIUS), type);
	}
	reversespiralDegree -= 15;
	if (reversespiralDegree == -360) reversespiralDegree = 0;
	spiralDegree += 15;
	if (spiralDegree == 360) spiralDegree = 0;
}

void BulletManager::ClockwiseSpriral (Vector3 pos, char* type) {
	for (int i = 0; i < 1; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + 180 + i * 15, pos.y, RADIUS), type);
	}
	spiralDegree += 15;
	if (spiralDegree == 360) spiralDegree = 0;
}

void BulletManager::Spiral(Vector3 pos, char* type) {
	for (int i = 0; i < 1; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + 180 + i * 15, pos.y, RADIUS), type);

		addBullet(pos.x, pos.y, xPixel(spiralDegree + 90 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + 90 + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree - 90 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree - 90 + i * 15, pos.y, RADIUS), type);
	}
	spiralDegree += 15;
	if (spiralDegree == 360) spiralDegree = 0;
}

void BulletManager::CounterClockwiseSpiral(Vector3 pos, char* type) {
	for (int i = 0; i < 1; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + i * 15, pos.y, RADIUS), type);
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + 180 + i * 15, pos.y, RADIUS), type);
	}
	spiralDegree -= 15;
	if (spiralDegree == -360) spiralDegree = 0;
}

void BulletManager::Twist(Vector3 pos, char* type) {
	for (int i = 0; i < 1; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 15, pos.x, RADIUS),
			yPixel(spiralDegree + i * 15, pos.y, RADIUS), type);
	}
	spiralDegree += 15;
}

void BulletManager::Triangle(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + 30 + 180, pos.x, RADIUS / 2);
	x_2 = xPixel(degree - 30 + 180, pos.x, RADIUS / 2);
	y_1 = yPixel(degree + 30 + 180, pos.y, RADIUS / 2);
	y_2 = yPixel(degree - 30 + 180, pos.y, RADIUS / 2);
	
	Vector2 root1 = PerpendicularLine(GameManager::GetInstance()->player->transform->position.x
		, GameManager::GetInstance()->player->transform->position.y, a, b);
	Vector2 root2 = PerpendicularLine(x_1, y_1, root1.x, root1.y);
	Vector2 root3 = PerpendicularLine(x_2, y_2, root1.x, root1.y);
	Vector2 root4 = Sys2EqSolve(root2.x, -1.0f, -root2.y, root1.x, -1.0f, -root1.y);
	Vector2 root5 = Sys2EqSolve(root3.x, -1.0f, -root3.y, root1.x, -1.0f, -root1.y);

	addBullet(x_1, y_1, root4.x, root4.y, type);
	addBullet(x_2, y_2, root5.x, root5.y, type);
	addBullet(pos.x, pos.y, GameManager::GetInstance()->player->transform->position.x,
		GameManager::GetInstance()->player->transform->position.y, type);
}

void BulletManager::X(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + 45, pos.x, RADIUS / 2);
	x_2 = xPixel(degree - 45, pos.x, RADIUS / 2);
	y_1 = yPixel(degree + 45, pos.y, RADIUS / 2);
	y_2 = yPixel(degree - 45, pos.y, RADIUS / 2);
	x_3 = xPixel(degree + 225, pos.x, RADIUS / 2);
	y_3 = yPixel(degree + 225, pos.y, RADIUS / 2);
	x_4 = xPixel(degree + 135, pos.x, RADIUS / 2);
	y_4 = yPixel(degree + 135, pos.y, RADIUS / 2);

	Vector2 root1 = PerpendicularLine(GameManager::GetInstance()->player->transform->position.x
		, GameManager::GetInstance()->player->transform->position.y, a, b);
	Vector2 root2 = PerpendicularLine(x_1, y_1, root1.x, root1.y);
	Vector2 root3 = PerpendicularLine(x_2, y_2, root1.x, root1.y);
	Vector2 root4 = Sys2EqSolve(root2.x, -1.0f, -root2.y, root1.x, -1.0f, -root1.y);
	Vector2 root5 = Sys2EqSolve(root3.x, -1.0f, -root3.y, root1.x, -1.0f, -root1.y);

	addBullet(x_1, y_1, root4.x, root4.y, type);
	addBullet(x_2, y_2, root5.x, root5.y, type);
	addBullet(pos.x, pos.y, GameManager::GetInstance()->player->transform->position.x,
		GameManager::GetInstance()->player->transform->position.y, type);
	addBullet(x_4, y_4, root4.x, root4.y, type);
	addBullet(x_3, y_3, root5.x, root5.y, type);
}

void BulletManager::Square(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + 45, pos.x, RADIUS / 2);
	x_2 = xPixel(degree - 45, pos.x, RADIUS / 2);
	y_1 = yPixel(degree + 45, pos.y, RADIUS / 2);
	y_2 = yPixel(degree - 45, pos.y, RADIUS / 2);
	x_3 = xPixel(degree + 225, pos.x, RADIUS / 2);
	y_3 = yPixel(degree + 225, pos.y, RADIUS / 2);
	x_4 = xPixel(degree + 135, pos.x, RADIUS / 2);
	y_4 = yPixel(degree + 135, pos.y, RADIUS / 2);

	Vector2 root1 = PerpendicularLine(GameManager::GetInstance()->player->transform->position.x
		, GameManager::GetInstance()->player->transform->position.y, a, b);
	Vector2 root2 = PerpendicularLine(x_1, y_1, root1.x, root1.y);
	Vector2 root3 = PerpendicularLine(x_2, y_2, root1.x, root1.y);
	Vector2 root4 = Sys2EqSolve(root2.x, -1.0f, -root2.y, root1.x, -1.0f, -root1.y);
	Vector2 root5 = Sys2EqSolve(root3.x, -1.0f, -root3.y, root1.x, -1.0f, -root1.y);

	addBullet(x_1, y_1, root4.x, root4.y, type);
	addBullet(x_2, y_2, root5.x, root5.y, type);
	addBullet(x_4, y_4, root4.x, root4.y, type);
	addBullet(x_3, y_3, root5.x, root5.y, type);
}

void BulletManager::Arc(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4, x_5, y_5;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + 15, pos.x, RADIUS);
	x_2 = xPixel(degree - 15, pos.x, RADIUS);
	y_1 = yPixel(degree + 15, pos.y, RADIUS);
	y_2 = yPixel(degree - 15, pos.y, RADIUS);
	x_3 = xPixel(degree + 45, pos.x, RADIUS);
	y_3 = yPixel(degree + 45, pos.y, RADIUS);
	x_4 = xPixel(degree - 45, pos.x, RADIUS);
	y_4 = yPixel(degree - 45, pos.y, RADIUS);
	x_5 = xPixel(degree, pos.x, RADIUS);
	y_5 = xPixel(degree, pos.y, RADIUS);

	Vector2 root1 = PerpendicularLine(GameManager::GetInstance()->player->transform->position.x
		, GameManager::GetInstance()->player->transform->position.y, a, b);
	Vector2 root2 = PerpendicularLine(x_1, y_1, root1.x, root1.y);
	Vector2 root3 = PerpendicularLine(x_2, y_2, root1.x, root1.y);
	Vector2 root6 = PerpendicularLine(x_3, y_3, root1.x, root1.y);
	Vector2 root7 = PerpendicularLine(x_4, y_4, root1.x, root1.y);
	Vector2 root4 = Sys2EqSolve(root2.x, -1.0f, -root2.y, root1.x, -1.0f, -root1.y);
	Vector2 root5 = Sys2EqSolve(root3.x, -1.0f, -root3.y, root1.x, -1.0f, -root1.y);
	Vector2 root8 = Sys2EqSolve(root6.x, -1.0f, -root6.y, root1.x, -1.0f, -root1.y);
	Vector2 root9 = Sys2EqSolve(root7.x, -1.0f, -root7.y, root1.x, -1.0f, -root1.y);

	addBullet(x_1, y_1, root4.x, root4.y, type);
	addBullet(x_2, y_2, root5.x, root5.y, type);
	addBullet(x_4, y_4, root9.x, root9.y, type);
	addBullet(x_3, y_3, root8.x, root8.y, type);
}

void BulletManager::SuperTrace(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + 30, pos.x, RADIUS / 2);
	x_2 = xPixel(degree - 30, pos.x, RADIUS / 2);
	y_1 = yPixel(degree + 30, pos.y, RADIUS / 2);
	y_2 = yPixel(degree - 30, pos.y, RADIUS / 2);

	x_3 = xPixel(degree + 60, pos.x, RADIUS / 2);
	x_4 = xPixel(degree - 60, pos.x, RADIUS / 2);
	y_3 = yPixel(degree + 60, pos.y, RADIUS / 2);
	y_4 = yPixel(degree - 60, pos.y, RADIUS / 2);

	addBullet(pos.x, pos.y, GameManager::GetInstance()->player->transform->position.x,
		GameManager::GetInstance()->player->transform->position.y, type);
	addBullet(pos.x, pos.y, x_1, y_1, type);
	addBullet(pos.x, pos.y, x_2, y_2, type);
	addBullet(pos.x, pos.y, x_3, y_3, type);
	addBullet(pos.x, pos.y, x_4, y_4, type);
}

void BulletManager::ShotGun(Vector3 pos, char* type) {
	float a, b, degree, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4;
	Vector2 root = Sys2EqSolve(pos.x, 1.0f, pos.y, GameManager::GetInstance()->player->transform->position.x
		, 1.0f, GameManager::GetInstance()->player->transform->position.y);
	a = root.x;	b = root.y;
	if (a < 0) degree = 180 - (atan(abs(a)) * 180 / PI);
	else degree = atan(abs(a)) * 180 / PI;
	if (GameManager::GetInstance()->player->transform->position.y < pos.y) degree = degree + 180;

	x_1 = xPixel(degree + offset, pos.x, RADIUS / 2 + offset);
	y_1 = yPixel(degree + offset, pos.y, RADIUS / 2 + offset);

	addBullet(pos.x, pos.y, x_1, y_1, type);
	if (offset == 60) shotgunState = 1;
	if (offset == -60) shotgunState = 0;
	if (shotgunState == 0) offset += 15;
	if (shotgunState == 1) offset -= 15;
}

void BulletManager::Trace(Vector3 pos, char* type) {
	addBullet(pos.x, pos.y, GameManager::GetInstance()->player->transform->position.x, 
		GameManager::GetInstance()->player->transform->position.y, type);
}

void BulletManager::DeleteAll() {

}

