#pragma once
class PerkDetails
{
public:
	PerkDetails();
	PerkDetails(char* blueprint);
	~PerkDetails();

	Blueprint* blueprint;
	int timeUsed = 0;
};

