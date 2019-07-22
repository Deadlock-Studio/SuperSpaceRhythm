#pragma once
#include "stdafx.h"
#include <vector>
#include "SoundManager.h"
#include "SFML/Audio.hpp"
#include "ResourceManager.h"
#include "HandyString.h"

SoundManager * SoundManager::ms_pInstance = NULL;

void SoundManager::DeleteAll() {
	for (std::vector<Track*>::iterator it = MusicList.begin(); it != MusicList.end(); ++it) {
		(*it)->music.stop();
		free((*it)->songname);
		delete[] (*it)->beatmap;
		delete (*it);
	}
	for (std::vector<Sfx*>::iterator it = SfxList.begin(); it != SfxList.end(); ++it) {
		(*it)->sound.stop();
		free((*it)->sfxname);
		delete (*it);
	}
}

void SoundManager::Init() {
	int tracknum, sfxnum, beatnum;
	float time;
	FILE* f = fopen("../Resources/Sound/Sound.txt", "r");
	if (f == nullptr)
	{
		printf("Failed to open Sound.txt\n");
		return;
	}
	fscanf(f, "MUSIC %d\n", &tracknum);
	for (int i = 0; i < tracknum; i++)
	{
		Track* temptrack = new Track;

		//load filename
		char* tempname = new char[300];
		fscanf(f, "FILE %s\n", tempname);
		temptrack->songname = strdup(HandyString::getFileName(string(tempname)).c_str());

		//load beat map
		char* beatmap = new char[300];
		fscanf(f, "MAP %d %s\n", &beatnum, beatmap);
		float* map = new float[beatnum];
		FILE* fi = fopen(beatmap, "r");
		for (int j = 0; j < beatnum; j++)
		{
			fscanf(fi, "%f\n", &time);
			map[j] = time;
		}
		fclose(fi);
		temptrack->beatmap = map;
		
		//init track player
		temptrack->music.openFromFile(tempname);
		temptrack->music.setLoop(true);
		MusicList.push_back(temptrack);
		delete[] beatmap;
		delete[] tempname;
	}
	fscanf(f, "SFX %d\n", &sfxnum);
	for (int i = 0; i < sfxnum; i++)
	{
		Sfx* tempsfx = new Sfx;

		//load filename
		char* tempname = new char[300];
		fscanf(f, "FILE %s\n", tempname);
		tempsfx->sfxname = strdup(HandyString::getFileName(string(tempname)).c_str());

		//init sfx player
		tempsfx->buffer.loadFromFile(tempname);
		tempsfx->sound.setBuffer(tempsfx->buffer);
		SfxList.push_back(tempsfx);
		delete[] tempname;
	}
	fclose(f);
};


SoundManager::Track* SoundManager::getTrack(char* filename) {
	for (std::vector<Track*>::iterator it = MusicList.begin(); it != MusicList.end(); ++it) {
		if (strcmp(filename, (*it)->songname) == 0) return (*it);
	}
	return NULL;
};

SoundManager::Sfx* SoundManager::getSfx(char* filename) {
	for (std::vector<Sfx*>::iterator it = SfxList.begin(); it != SfxList.end(); ++it) {
		if (strcmp(filename, (*it)->sfxname) == 0) return (*it);
	}
	return NULL;
};

void SoundManager::Calibrate() {};