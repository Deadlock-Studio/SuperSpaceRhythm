#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "SoundManager.h"
#include "SFML/Audio.hpp"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "HandyString.h"
#include <algorithm>

SoundManager * SoundManager::ms_pInstance = NULL;
//ofstream fout("SongPos.txt");
int sp = 0;
bool del = FALSE;

float Difference(float a, float b) {
	return abs(a - b);
}

bool PositionComparision(float a, float b) {
	float diff = Difference(a, b);
	if (diff <= 0.02)
	{
		return TRUE;
	}
	return FALSE;
}

void SoundManager::DeleteAll() {
	for (std::vector<Track*>::iterator it = MusicList.begin(); it != MusicList.end(); ++it) {
		(*it)->music.stop();
		free((*it)->songname);
		delete[](*it)->beatmap;
		delete (*it);
	}
	for (std::vector<Sfx*>::iterator it = SfxList.begin(); it != SfxList.end(); ++it) {
		(*it)->sound.stop();
		free((*it)->sfxname);
		delete (*it);
	}
	for (std::deque<Beat*>::iterator it = BeatList.begin(); it != BeatList.end(); ++it)
	{
		delete (*it);
	}
	BeatList.clear();
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

	//load theme songs
	fscanf(f, "MUSIC %d\n", &tracknum);
	for (int i = 0; i < tracknum; i++)
	{
		Track* temptrack = new Track;

		//load filename
		char* tempname = new char[300];
		fscanf(f, "FILE %s\n", tempname);
		temptrack->songname = _strdup(HandyString::getFileName(string(tempname)).c_str());

		//load beat map
		char* beatmap = new char[300];
		fscanf(f, "MAP %d %s\n", &beatnum, beatmap);
		temptrack->beatnum = beatnum;
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

	//load sound effects
	fscanf(f, "SFX %d\n", &sfxnum);
	for (int i = 0; i < sfxnum; i++)
	{
		Sfx* tempsfx = new Sfx;

		//load filename
		char* tempname = new char[300];
		fscanf(f, "FILE %s\n", tempname);
		tempsfx->sfxname = _strdup(HandyString::getFileName(string(tempname)).c_str());

		//init sfx player
		tempsfx->buffer.loadFromFile(tempname);
		tempsfx->sound.setBuffer(tempsfx->buffer);
		SfxList.push_back(tempsfx);
		delete[] tempname;
	}
	fclose(f);

	SpawnBeatGUI();
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

void SoundManager::safePop(std::deque<Beat*>* queue) {
	Beat* temp = queue->front();
	queue->pop_front();
	temp->Delete();
	delete temp;
}
void SoundManager::SpawnBeatGUI()
{
	//render rhythm grid
	yeet = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("yeet"), Vector3(Globals::screenWidth / 2, Globals::screenHeight / 6, GUI_LAYER), Vector3(1, 1, 1), Vector3());
	bruh = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("bruh"), Vector3(Globals::screenWidth / 2, Globals::screenHeight / 6, GUI_LAYER), Vector3(1, 1, 1), Vector3());
	meh = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("meh"), Vector3(Globals::screenWidth / 2, Globals::screenHeight / 6, GUI_LAYER), Vector3(1, 1, 1), Vector3());
	yeet->GetComponent<SpriteRenderer>()->isActive = FALSE;
	bruh->GetComponent<SpriteRenderer>()->isActive = FALSE;
	meh->GetComponent<SpriteRenderer>()->isActive = FALSE;
	SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("grid"), Vector3(Globals::screenWidth / 2, Globals::screenHeight / 15, GUI_LAYER), Vector3(10, 1, 1), Vector3());
	SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("ring"), Vector3(Globals::screenWidth / 2, Globals::screenHeight / 15, GUI_LAYER), Vector3(1, 1, 1), Vector3());
};

bool SoundManager::CalibrateAudio() {
	if (cal <= 19)
	{
		counter++;
		if (counter == 30)
		{
			SoundManager::GetInstance()->getSfx("hit")->sound.play();
			play = GetTickCount();
			counter = 0;
		}
		if (InputManager::GetInstance()->isMouseDown)
		{
			if (!hit)
			{
				press = GetTickCount();
				timing[cal] = (float)(press - play);
				cal++;
				hit = TRUE;
			}
		}
		else hit = FALSE;
		return FALSE;
	}
	else {
		for (int i = 0; i < 20; i++)
		{
			total += timing[i];
		}
		AudioOffset = (total / 20.0f) / 1000.0f;
		counter = 0;
		cal = 0;
		return TRUE;
	}
};

bool SoundManager::CalibrateVisual() {
	if (cal <= 19)
	{
		counter++;
		if (counter == 30)
		{
			calibration = new Beat(10, FALSE);
			play = GetTickCount();
			counter = 0;
			sp++;
			del = FALSE;
		}
		if (InputManager::GetInstance()->isMouseDown)
		{
			if (!hit)
			{
				press = GetTickCount();
				timing[cal] = (float)(press - play - AudioOffset * 80 / 100);
				cal++;
				hit = TRUE;
			}
		}
		else hit = FALSE;
		if (sp > 0 && sp < 5) sp++;
		if (sp == 5)
		{
			del = TRUE;
			calibration->Delete();
			delete calibration;
			sp = 0;
		}
		return FALSE;
	}
	else {
		for (int i = 0; i < 20; i++)
		{
			total += timing[i];
		}
		if (!del)
		{
			calibration->Delete();
			delete calibration;
		}
		VisualOffset = (total / 20.0f) / 1000.0f;
		counter = 0;
		cal = 0;
		return TRUE;
	}
}

int SoundManager::RhythmConductor(Track* track, float FrameTime) {
	sf::Time temp;
	signal = 0;
	enemySignal = 0;
	//initialization
	if (track->music.getStatus() == sf::Sound::Stopped)
	{
		for (int i = 0; i < track->beatnum; i++)
		{
			track->beatmap[i] = track->beatmap[i] + AudioOffset;
		}
		for (std::deque<Beat*>::iterator it = BeatList.begin(); it != BeatList.end(); ++it)
		{
			delete (*it);
		}
		yeet->GetComponent<SpriteRenderer>()->isActive = FALSE;
		bruh->GetComponent<SpriteRenderer>()->isActive = FALSE;
		meh->GetComponent<SpriteRenderer>()->isActive = FALSE;
		BeatList.clear();
		track->index = 0;
		track->music.setVolume(20);
		track->lastPos = 0;
		track->songTime = 0;
		track->beattime = track->beatmap[1] - track->beatmap[0];
		track->distance = SPEED * track->beattime * 60;
		beatonscreen = (int)std::ceil(Globals::X_percent(50) / track->distance);
		InitPosX = beatonscreen * track->distance;
		track->music.setLoop(false);
		track->prevBeatTime = track->beatmap[0];
		track->music.play();
		float tm = track->music.getPlayingOffset().asSeconds();

		while (!PositionComparision(tm, VisualOffset))
		{
			tm = track->music.getPlayingOffset().asSeconds();
		}

		for (int i = 1; i <= beatonscreen; i++)
		{
			Beat *temp;
			if (i % 4 == 0)
			{
				temp = new Beat(InitPosX, track->beatmap[track->index], TRUE);
			}
			else
				temp = new Beat(InitPosX, track->beatmap[track->index], FALSE);
			BeatList.push_back(temp);
			track->index++;
		}
	}

	else {
		float tm = track->music.getPlayingOffset().asSeconds();
		time = GetTickCount();
		track->songTime += 1.0f/FPS;

		if (yeet->GetComponent<SpriteRenderer>()->isActive == TRUE || bruh->GetComponent<SpriteRenderer>()->isActive == TRUE ||
			meh->GetComponent<SpriteRenderer>()->isActive == TRUE)
		{
			counter++;
			if (counter == 5) {
				yeet->GetComponent<SpriteRenderer>()->isActive = FALSE;
				bruh->GetComponent<SpriteRenderer>()->isActive = FALSE;
				meh->GetComponent<SpriteRenderer>()->isActive = FALSE;
				counter = 0;
			}
		}

		//check input
		if (InputManager::GetInstance()->isMouseDown || InputManager::GetInstance()->ActionCheck(Dash) || InputManager::GetInstance()->ActionCheck(Reform))
		{
			float greatGateEarly, greatGateLate, okGateEarly, okGateLate, delGate;
			if (!hit)
			{
				if (BeatList.size() > 0)
				{
					greatGateLate = track->prevBeatTime + GREAT_GATE * track->beattime;
					okGateLate = track->prevBeatTime + OK_GATE * track->beattime;

					if (BeatList.front()->checkActive())
					{
						greatGateEarly = BeatList.front()->beatPos - GREAT_GATE * track->beattime;
						okGateEarly = BeatList.front()->beatPos - OK_GATE * track->beattime;
						delGate = BeatList.front()->beatPos - DEL_GATE * track->beattime;
					}
					else {
						greatGateEarly = BeatList.at(1)->beatPos - GREAT_GATE * track->beattime;
						okGateEarly = BeatList.at(1)->beatPos - OK_GATE * track->beattime;
						delGate = BeatList.at(1)->beatPos - DEL_GATE * track->beattime;
					}

					if (track->songTime >= okGateEarly && track->songTime <= greatGateEarly)
					{
						if (BeatList.front()->hidden == FALSE)
						{
							if (BeatList.front()->halftime == TRUE && InputManager::GetInstance()->ActionCheck(Reform))
							{
								signal = 4;
								halftime = !halftime;
							}
							else
							{
								meh->GetComponent<SpriteRenderer>()->isActive = TRUE;
								signal = 2;
							}
						}
					}
					else if (track->songTime >= greatGateLate && track->songTime <= okGateLate)
					{
						if (track->prevBeatStat == FALSE)
						{
							if (InputManager::GetInstance()->ActionCheck(Reform) && track->prevStat == TRUE)
							{
								signal = 4;
								halftime = !halftime;
							}
							else
							{
								meh->GetComponent<SpriteRenderer>()->isActive = TRUE;
								signal = 2;
							}
						}
					}
					else if (track->songTime > greatGateEarly)
					{
						if (BeatList.front()->hidden == FALSE)
						{
							if (BeatList.front()->halftime == TRUE && InputManager::GetInstance()->ActionCheck(Reform))
							{
								signal = 4;
								halftime = !halftime;
							}
							else
							{
								yeet->GetComponent<SpriteRenderer>()->isActive = TRUE;
								signal = 3;
							}
						}
					}
					else if (track->songTime < greatGateLate)
					{
						if (track->prevBeatStat == FALSE)
						{
							if (InputManager::GetInstance()->ActionCheck(Reform) && track->prevStat == TRUE)
							{
								signal = 4;
								halftime = !halftime;
							}
							else
							{
								yeet->GetComponent<SpriteRenderer>()->isActive = TRUE;
								signal = 3;
							}
						}
					}
					else
					{
						if (track->prevBeatStat == FALSE || BeatList.front()->hidden == FALSE)
						{
							bruh->GetComponent<SpriteRenderer>()->isActive = TRUE;
							signal = 1;
						}
					}

					if (track->songTime >= delGate)
					{
						BeatList.front()->Hide();
					}
					hit = TRUE;
				}
			}
		}
		else hit = FALSE;

		//beat check
		if (BeatList.size() != 0)
		{
			if (PositionComparision(track->songTime, BeatList.front()->beatPos))
			{
				enemySignal = 1;
				if (BeatList.front()->halftime == TRUE)
				{
					enemySignal = 2;
				}
				track->prevBeatTime = BeatList.front()->beatPos;
				track->prevStat = BeatList.front()->halftime;
				track->prevBeatStat = BeatList.front()->hidden;
				safePop(&BeatList);
				if (track->index <= track->beatnum)
				{
					Beat *tmp;

					if (halftime == FALSE)
					{
						if ((track->index - 7) % 4 == 0)
						{
							tmp = new Beat(InitPosX, track->beatmap[track->index], TRUE);
						}
						else
							tmp = new Beat(InitPosX, track->beatmap[track->index], FALSE);
					}
					else
					{
						if ((track->index - 7) % 4 == 0)
						{
							if (halftimeCounter == 3)
							{
								tmp = new Beat(InitPosX, track->beatmap[track->index], TRUE);
								halftimeCounter = 0;
							}
							else
							{
								tmp = new Beat(InitPosX, track->beatmap[track->index], FALSE);
								halftimeCounter++;
							}
						}
						else
						{
							tmp = new Beat(InitPosX, track->beatmap[track->index], FALSE);
							tmp->Hide();
						}
					}

					BeatList.push_back(tmp);
					track->index++;
				}
			}

			//move
			for (size_t i = 0; i < BeatList.size(); i++)
			{
				BeatList[i]->Move(track->songTime);
			}
		}
	}
	return signal;
}