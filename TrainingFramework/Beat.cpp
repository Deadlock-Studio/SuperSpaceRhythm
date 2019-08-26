#include "stdafx.h"
#include "Beat.h"
#include "SoundManager.h"

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

Beat::Beat(float X_offset, int beatIndex, float beatpos, bool type)
{
	halftime = type;
	if (!halftime)
	{
		left = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-left"), Vector3(Globals::X_percent(50) - X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3());
		right = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-right"), Vector3(Globals::X_percent(50) + X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3(0, 0, 0));
	}
	if (halftime)
	{
		left = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-left-ht"), Vector3(Globals::X_percent(50) - X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3());
		right = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-right-ht"), Vector3(Globals::X_percent(50) + X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3(0, 0, 0));
	}
	leftSpawnPos = Globals::X_percent(50) - X_offset;
	rightSpawnPos = Globals::X_percent(50) + X_offset;
	distance = X_offset;
	this->beatPos = beatpos;
	this->beatIndex = beatIndex;
}

Beat::Beat(float X_offset, bool type) {
	halftime = type;
	if (!halftime)
	{
		left = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-left"), Vector3(Globals::X_percent(50) - X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3());
		right = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-right"), Vector3(Globals::X_percent(50) + X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3(0, 0, 0));
	}
	if (halftime)
	{
		left = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-left-ht"), Vector3(Globals::X_percent(50) - X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3());
		right = SceneManager::GetInstance()->SpawnGUI(SceneManager::GetInstance()->GetBlueprintByName("beat-right-ht"), Vector3(Globals::X_percent(50) + X_offset, Globals::screenHeight / 15, 2), Vector3(1, 1, 1), Vector3(0, 0, 0));
	}
}

void Beat::Delete() {
	SceneManager::GetInstance()->safeDel_k(left);
	SceneManager::GetInstance()->safeDel_k(right);
}

void Beat::Hide() {
	hidden = TRUE;
	left->GetComponent<SpriteRenderer>()->isActive = false;
	right->GetComponent<SpriteRenderer>()->isActive = false;
}

void Beat::Show() {
	left->GetComponent<SpriteRenderer>()->isActive = true;
	right->GetComponent<SpriteRenderer>()->isActive = true;
}

bool Beat::checkActive() {
	if (left->GetComponent<SpriteRenderer>()->isActive && right->GetComponent<SpriteRenderer>()->isActive)
	{
		return TRUE;
	}
	return FALSE;
}

void Beat::Stop() {
	left->isActive = FALSE;
	right->isActive = FALSE;
}

Beat::~Beat()
{
}

void Beat::Move(float songtime) {
	left->UpdatePosition(lerp(leftSpawnPos, Globals::X_percent(50), (2 - (beatPos - songtime)) / 2), left->transform->position.y, left->transform->position.z);
	right->UpdatePosition(lerp(rightSpawnPos, Globals::X_percent(50), (2 - (beatPos - songtime)) / 2), right->transform->position.y, right->transform->position.z);
}