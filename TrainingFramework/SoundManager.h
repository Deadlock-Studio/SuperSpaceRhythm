#pragma once
#include "SFML/Audio.hpp"
#include <vector>

class SoundManager
{
public:
	typedef struct Track {
		char* songname;
		float* beatmap;
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
	void Calibrate();

protected:
	static SoundManager * ms_pInstance;

public:
	float offset;
	std::vector<Track*> MusicList;
	std::vector<Sfx*> SfxList;
};

