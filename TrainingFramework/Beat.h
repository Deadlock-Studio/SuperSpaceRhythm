#include "stdafx.h"
#pragma once

class Beat
{
public:
	Beat(float X_offset, float beatpos, bool type);
	~Beat();
	Beat(float X_offset, bool type);
	void Move(float songtime);
	void Hide();
	void Show();
	bool checkActive();
	void Delete();
	void Stop();

public:
	GUI* left;
	GUI* right;
	bool halftime = FALSE;
	bool hidden = FALSE;
	float leftSpawnPos;
	float rightSpawnPos;
	float beatPos;
	float distance;
};

