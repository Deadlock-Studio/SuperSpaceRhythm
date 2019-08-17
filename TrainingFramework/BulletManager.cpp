#include "stdafx.h"
#include "BulletManager.h"

int spiralDegree = 0;
int reversespiralDegree = 0;

BulletManager * BulletManager::ms_pInstance = NULL;

float xPixel(float degree, float posX) {
	return (float)(posX + RADIUS * cos(degree * PI / 180));
}

float yPixel(float degree, float posY) {
	return (float)(posY + RADIUS * sin(degree * PI / 180));
}

void addBullet(float x, float y, float xPixel, float yPixel) {
	SceneManager::GetInstance()->SpawnBullet(x, y, xPixel, yPixel, "eBullet");
}

void BulletManager::Circle(Vector3 pos) {
	for (int i = 0; i < 360; i += 30)
	{
		addBullet(pos.x, pos.y, xPixel((float)i, pos.x), yPixel((float)i, pos.y));
	}
}

void BulletManager::Cross(Vector3 pos) {
	for (int i = 0; i < 360; i+=90)
	{
		addBullet(pos.x, pos.y, xPixel((float)i, pos.x), yPixel((float)i, pos.y));
	}
}

void BulletManager::Flower(Vector3 pos) {
	for (int i = 0; i < 2; i++) {
		addBullet(pos.x, pos.y, xPixel(spiralDegree + i * 90, pos.x),
			yPixel(spiralDegree + i * 90, pos.y));
		addBullet(pos.x, pos.y, xPixel(spiralDegree + 180 + i * 90, pos.x),
			yPixel(spiralDegree + 180 + i * 90, pos.y));
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + i * 90, pos.x),
			yPixel(reversespiralDegree + i * 90, pos.y));
		addBullet(pos.x, pos.y, xPixel(reversespiralDegree + 180 + i * 90, pos.x),
			yPixel(reversespiralDegree + 180 + i * 90, pos.y));
	}
	reversespiralDegree -= 10;
	spiralDegree += 10;
	if (spiralDegree == 360) spiralDegree = 0;
	if (reversespiralDegree == -360) spiralDegree = 0;
}

void BulletManager::DeleteAll() {

}