#pragma once
#include <stdlib.h> 
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
#define MOVE_SPEED 250.0f
#define PLAYER_WALK_SPEED 50.0f
#define GRID_SIZE 40.0f
#define FPS 60.0f
#define X_MIN 68.76f
#define X_MAX 1209.25f
#define Y_MIN -184.25f
#define Y_MAX 949.25f
#define NONE_SPAWNING_RADIUS 120
#define SPAWN_DEPTH 150
#define WALL_NONE_SPAWN 130

class Globals
{
public:
	static bool chance(int percent) {
		return (rand() % 100) < percent;
	}
	static const int screenWidth = 1280;
	static const int screenHeight = 720;
	static const int velIter = 8;
	static const int posIter = 3;
	static float X_percent(float x) {
		return screenWidth * x / 100;
	}
	static float Y_percent(float y) {
		return screenHeight * y / 100;
	}
	static float clamp_x(float x) {
		if (x > X_MAX - WALL_NONE_SPAWN)
		{
			return X_MAX - WALL_NONE_SPAWN;
		}
		else if (x < X_MIN + WALL_NONE_SPAWN)
		{
			return X_MIN + WALL_NONE_SPAWN;
		}
		else return x;
	}
	static float clamp_y(float y) {
		if (y > Y_MAX - WALL_NONE_SPAWN)
		{
			return Y_MAX - WALL_NONE_SPAWN;
		}
		else if (y < Y_MIN + WALL_NONE_SPAWN)
		{
			return Y_MIN + WALL_NONE_SPAWN;
		}
		else return y;
	}
};