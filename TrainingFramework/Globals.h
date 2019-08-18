#pragma once
#define UPPER_GUI_LAYER 7
#define GUI_LAYER 6
#define EFFECT_LAYER 5
#define PLAYER_LAYER 4
#define BULLET_LAYER 3
#define MOB_LAYER 2
#define ITEM_LAYER 1
#define TRAP_LAYER -1
#define ROOM_LAYER -2
#define BG_LAYER -3
#define PIXEL_RATIO 50.0f
#define MOVE_SPEED 500.0f
#define GRID_SIZE 40.0f
#define FPS 60.0f

class Globals
{
public:
	static const int screenWidth = 1280;
	static const int screenHeight = 720;
	static const int velIter = 6;
	static const int posIter = 2;
	static float X_percent(float x) {
		return screenWidth * x / 100;
	}
	static float Y_percent(float y) {
		return screenHeight * y / 100;
	}
};