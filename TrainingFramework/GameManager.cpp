#include "stdafx.h"
#include "GameManager.h"
#include "Control.h"
#include "GUI.h"
#include "Button.h"

GameManager * GameManager::ms_pInstance = NULL;

void GameManager::Init()
{
	SetState(&GameManager::Loading);

	LoadConstructorMap();
	
	camera = SceneManager::GetInstance()->usedCamera;

	splashScreen = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("splash_screen"),
		Vector3(640, 360, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	splashScreen->GetComponent<SpriteRenderer>()->alpha = 0;
	splashScreen->isActive = false;

	playButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("play_button"),
		Vector3(640, 350, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	playButton->GetComponent<SpriteRenderer>()->alpha = 0;
	playButton->isActive = false;

	optionsButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("options_button"),
		Vector3(640, 200, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	optionsButton->GetComponent<SpriteRenderer>()->alpha = 0;
	optionsButton->isActive = false;

	menuBG = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("menu_BG"),
		Vector3(640, 360, BG_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	menuBG->GetComponent<SpriteRenderer>()->alpha = 0;
	menuBG->isActive = false;

	title = TextManager::GetInstance()->AddText("monogram", "Super Space Rhythm", Vector4(0.776, 0.4039, 0.5529, 1.0), 350, 500, 2, 2);
	title->color.w = 0;
	title->isActive = false;
}

void GameManager::LoadConstructorMap()
{
	constructorMap["object"] = &constructor<GameObject>;
	constructorMap["player"] = &constructor<Player>;
	constructorMap["boss"] = &constructor<Boss>;
	constructorMap["hp"] = &constructor<HealthPotion>;
	constructorMap["trap"] = &constructor<Trap>;
	constructorMap["crate"] = &constructor<Crate>;
	constructorMap["mob"] = &constructor<Mob1>;
	constructorMap["explosion"] = &constructor<Explosion>;
	constructorMap["tnt"] = &constructor<TNT>;
	constructorMap["mobShoot"] = &constructor<MobShoot>;
	constructorMap["mobExplode"] = &constructor<MobExplode>;
	constructorMap["mobExplodeBullet"] = &constructor<MobExplodeBullet>;
	constructorMap["mobKnight"] = &constructor<MobKnight>;
	constructorMap["mobWiz"] = &constructor<MobWiz>;
	constructorMap["mobNecro"] = &constructor<MobNecro>;
	constructorMap["mobDino"] = &constructor<MobDino>;
	constructorMap["shield"] = &constructor<Shield>;
	constructorMap["spawn"] = &constructor<SpawnEffect>;
	constructorMap["room"] = &constructor<Room>;
}

void GameManager::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)(deltaTime);

	//Game clock
	time += deltaTime;
}

GameObject* GameManager::Spawn(string constructorName, Blueprint* blueprint, Vector3 pos, Vector3 scale, Vector3 rotation)
{
	if (blueprint == NULL) {
		cout << "Can not spawn null object" << endl;
		return NULL;
	}

	if (!constructorMap.count(constructorName)) {
		cout << "There is no such constructor" << endl;
		return NULL;
	}

	GameObject* obj = constructorMap[constructorName](blueprint, pos, scale, rotation);
	SceneManager::GetInstance()->objectList.push_back(obj);

	return obj;
}

//Splash screen and load game resources
void GameManager::Loading(float deltaTime)
{
	
	//Transitions
	if (InputManager::GetInstance()->keyMap != 0) {
		//Skip splash screen
		UnloadAll();
		SetState(&GameManager::Menu);
		return;
	}

	//State Execution
	splashScreen->isActive = true;
	if (time > 0.5)
		splashScreen->GetComponent<SpriteRenderer>()->FadeIn(0.5);

	if (time > 4.5) {
		splashScreen->GetComponent<SpriteRenderer>()->FadeOut(0.5);
		if (splashScreen->GetComponent<SpriteRenderer>()->alpha == 0) {
			UnloadAll();
			SetState(&GameManager::Menu);
		}
	}
}

void GameManager::Menu(float deltaTime)
{
	//Transitions
	if (InputManager::GetInstance()->ActionCheck(DebugMode)) {
		UnloadAll();
		SetState(&GameManager::LoadDebugRoom);
		return;
	}

	//State Execution
	playButton->isActive = true;
	optionsButton->isActive = true;
	menuBG->isActive = true;
	title->isActive = true;
	if (time > 0.5) {
		playButton->GetComponent<SpriteRenderer>()->FadeIn(0.5);
		optionsButton->GetComponent<SpriteRenderer>()->FadeIn(0.5);
		menuBG->GetComponent<SpriteRenderer>()->FadeIn(0.5);
		title->FadeIn(0.5);
	}
	
	if (playButton->GetComponent<Button>()->isClicked)
		playPressed = true;

	if (playPressed) {
		playButton->GetComponent<SpriteRenderer>()->FadeOut(0.5);
		optionsButton->GetComponent<SpriteRenderer>()->FadeOut(0.5);
		menuBG->GetComponent<SpriteRenderer>()->FadeOut(0.5);
		title->FadeOut(0.5);

		if (playButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			UnloadAll();
			SetState(&GameManager::LoadRoom);
		}
	}
}

void GameManager::LoadDebugRoom(float deltaTime)
{
	room = Spawn("room",
		SceneManager::GetInstance()->GetBlueprintByName("demo_room"),
		Vector3(640, 360, ROOM_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	player = Spawn("player",
		SceneManager::GetInstance()->GetBlueprintByName("player"),
		Vector3(640, 360, PLAYER_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	//Spawn("boss",
	//	SceneManager::GetInstance()->GetBlueprintByName("boss"),
	//	Vector3(640, 360, PLAYER_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());

	//Spawn("hp",
	//	SceneManager::GetInstance()->GetBlueprintByName("health_potion"),
	//	Vector3(800, 300, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	Spawn("mob",
		SceneManager::GetInstance()->GetBlueprintByName("mob_white"),
		Vector3(800, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mob",
		SceneManager::GetInstance()->GetBlueprintByName("mob_red"),
		Vector3(850, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mob",
		SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
		Vector3(750, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	Spawn("mobExplode",
		SceneManager::GetInstance()->GetBlueprintByName("mob_explode"),
		Vector3(600, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobExplodeBullet",
		SceneManager::GetInstance()->GetBlueprintByName("mob_explode_bullet"),
		Vector3(500, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobShoot",
		SceneManager::GetInstance()->GetBlueprintByName("mob_mask"),
		Vector3(700, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobKnight",
		SceneManager::GetInstance()->GetBlueprintByName("mob_knight"),
		Vector3(400, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobWiz",
		SceneManager::GetInstance()->GetBlueprintByName("mob_wiz"),
		Vector3(300, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobNecro",
		SceneManager::GetInstance()->GetBlueprintByName("mob_necro"),
		Vector3(200, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	Spawn("mobDino",
		SceneManager::GetInstance()->GetBlueprintByName("mob_dino"),
		Vector3(100, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	//Spawn("trap",
	//	SceneManager::GetInstance()->GetBlueprintByName("trap"),
	//	Vector3(800, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(500, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(550, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(600, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(750, 700, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(800, 700, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(850, 700, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(900, 700, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//Spawn("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(650, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());


	SetState(&GameManager::RoomStart);
}

void GameManager::RoomStart(float deltaTime)
{
	camera->UpdatePosition(player->transform->position.x - (float)Globals::screenWidth / 2, player->transform->position.y - (float)Globals::screenHeight / 2, deltaTime);

}

void GameManager::LoadRoom(float deltaTime)
{
}

//Reset all objects to pos-Init() state
void GameManager::UnloadAll()
{
	time = 0;

	splashScreen->GetComponent<SpriteRenderer>()->alpha = 0;
	splashScreen->GetComponent<SpriteRenderer>()->FadeOff();
	splashScreen->isActive = false;

	playButton->GetComponent<SpriteRenderer>()->alpha = 0;
	playButton->GetComponent<SpriteRenderer>()->FadeOff();
	playButton->isActive = false;

	optionsButton->GetComponent<SpriteRenderer>()->alpha = 0;
	optionsButton->GetComponent<SpriteRenderer>()->FadeOff();
	optionsButton->isActive = false;

	menuBG->GetComponent<SpriteRenderer>()->alpha = 0;
	menuBG->GetComponent<SpriteRenderer>()->FadeOff();
	menuBG->isActive = false;

	title->color.w = 0;
	title->FadeOff();
	title->isActive = false;
}
