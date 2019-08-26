#pragma once
#include "SFML/Audio.hpp"
#include "Beat.h"
#include <vector>
#include <deque>
#define GREAT_GATE 0.1f
#define OK_GATE 0.2f
#define DEL_GATE 0.5f
#define SPEED 5

class SoundManager
{
public:
	typedef struct Track {
		char* songname;
		float* beatmap;
		float beattime;
		float prevBeatTime;
		bool prevStat;
		bool prevBeatStat = FALSE;
		float songTime;
		int index = 0;
		float lastPos = 0;
		int beatnum;
		float distance;
		sf::Music music;
	}Track;

	typedef struct Sfx {
		char* sfxname;
		sf::SoundBuffer buffer;
		sf::Sound sound;
	}Sfx;

protected:
	SoundManager() {}
	~SoundManager() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	SoundManager(const SoundManager &) {}
	SoundManager& operator =(const SoundManager &) {}

public:
	static void CreateInstance()
	{
		if (ms_pInstance == NULL)
			ms_pInstance = new SoundManager;
	}
	static SoundManager * GetInstance()
	{
		return ms_pInstance;
	}
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			GetInstance()->DeleteAll();
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}

	//Functions
	void Init();
	void DeleteAll();
	Track* getTrack(char* filename);
	Sfx* getSfx(char* filename);
	bool CalibrateAudio();
	bool CalibrateVisual();
	int RhythmConductor(Track* track, float FrameTime);
	void safePop(std::deque<Beat*>* queue);

	void SpawnBeatGUI();

protected:
	static SoundManager * ms_pInstance;

public:
	Track* currentlyPlaying;
	bool startConductor = TRUE;
	float offset;
	float AudioOffset = 0.0f;
	float VisualOffset = 0.0f;
	bool halftime = FALSE;
	int halftimeCounter = 0;
	int cal = 0;
	int signal;
	int signalPass = 0;
	int enemySignal = 0;
	int beatonscreen;
	float InitPosX;
	GUI * perfect;
	GUI* miss;
	GUI* good;
	GUI* goodPower;
	std::deque<Beat*> BeatList;
	std::vector<Track*> MusicList;
	std::vector<Sfx*> SfxList;
	bool hit = FALSE;
	bool destroyed = FALSE;
	bool audioCalibrated = FALSE;
	bool visualCalibrated = FALSE;
	int counter = 0;
	float timing[20], total = 0;
	DWORD play, press, begin, time;
	int musicVolume;
	Beat* calibration;
};

