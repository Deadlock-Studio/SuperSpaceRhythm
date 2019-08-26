#pragma once
#include <vector>

typedef struct spawnInfo {
	float time;
	char* constructor;
	char* blueprint;
	int layer;
};

class Script
{
public:
	Script();
	~Script();

	char* name;
	int clearCount = 0;
	vector<spawnInfo> spawnInfos;
};

