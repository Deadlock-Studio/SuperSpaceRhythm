#include "stdafx.h"
#include "GameManager.h"
#include "Control.h"
#include "Perk.h"
#include "GUI.h"
#include "Button.h"
#include "PerkDetails.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

#define FADE_SPEED 2

bool isMusicPlusPressed = false;
bool isMusicMinusPressed = false;

GameManager * GameManager::ms_pInstance = NULL;

void GameManager::Init()
{
	srand(time(NULL));
	LoadBuffList();
	LoadRoomList();
	LoadConstructorMap();
	SetState(&GameManager::Loading);

}

void GameManager::LoadBuffList()
{
	perkList.push_back(new PerkDetails("health_buff"));
	perkList.push_back(new PerkDetails("void_shell_buff"));
	perkList.push_back(new PerkDetails("shield_buff"));
	perkList.push_back(new PerkDetails("magic_beat_buff"));
	perkList.push_back(new PerkDetails("side_gun_buff"));
	perkList.push_back(new PerkDetails("enhanced_muzzle_buff"));
	perkList.push_back(new PerkDetails("blade_mail_buff"));
	perkList.push_back(new PerkDetails("invincible_cloak_buff"));
	perkList.push_back(new PerkDetails("swift_slipper_buff"));
	perkList.push_back(new PerkDetails("enhanced_shell_buff"));
}

void GameManager::LoadRoomList()
{
	roomList[0] = { "room_1", 0 };
	roomList[1] = { "room_2", 0 };
	roomList[2] = { "room_3", 0 };
	roomList[3] = { "room_4", 0 };
	roomList[4] = { "room_5", 0 };
	roomList[5] = { "room_6", 0 };
	roomList[6] = { "room_7", 0 };
	roomList[7] = { "room_8", 0 };
	roomList[8] = { "room_9", 0 };
}

void GameManager::LoadConstructorMap()
{
	constructorMap["object"] = &constructor<GameObject>;
	constructorMap["player"] = &constructor<Player>;
	constructorMap["boss"] = &constructor<Boss>;
	constructorMap["hp"] = &constructor<HealthPotion>;
	constructorMap["trap"] = &constructor<Trap>;
	constructorMap["mine"] = &constructor<Mine>;
	constructorMap["crate"] = &constructor<Crate>;
	constructorMap["mob"] = &constructor<MobSlime>;
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
	constructorMap["bulletDespawn"] = &constructor<BulletDespawn>;
	constructorMap["smoke"] = &constructor<Smoke>;
	constructorMap["room"] = &constructor<Room>;
	constructorMap["shopKeeper"] = &constructor<ShopKeeper>;
	constructorMap["ladder"] = &constructor<Ladder>;
	constructorMap["perk"] = &constructor<Perk>;
}

void GameManager::Update(float deltaTime)
{
	if (activeState != NULL)
		(this->*activeState)(deltaTime);

	//Game clock
	counter += deltaTime;
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

GameObject * GameManager::SpawnToRoom(string constructorName, Blueprint * blueprint, Vector3 pos, Vector3 scale, Vector3 rotation)
{
	GameObject* obj = Spawn(constructorName, blueprint, pos, scale, rotation);
	roomObject.push_back(obj);
	return obj;
}

void GameManager::ResetRoomAndBuff()
{
	for (int i = 0; i < sizeof(roomList) / sizeof(roomList[0]); i++) {
		roomList[i].timeUsed = 0;
	}

	for (std::vector<PerkDetails*>::iterator it = perkList.begin(); it != perkList.end(); ++it)
		(*it)->timeUsed = 0;
}

void GameManager::UnloadObjects()
{
	for (std::list<GameObject*>::iterator it = roomObject.begin(); it != roomObject.end(); ++it)
		SceneManager::GetInstance()->addToRemovalList(*it);
	roomObject.clear();
}

void GameManager::UnloadAll()
{
	UnloadObjects();

	for (std::vector<PerkDetails*>::iterator it = perkList.begin(); it != perkList.end(); ++it)
		delete *it;
	perkList.clear();
}

void GameManager::addToRemovalList(GameObject * key)
{
	SceneManager::GetInstance()->addToRemovalList(key);
	roomObject.remove(key);
}

//Splash screen and load game resources
void GameManager::Loading(float deltaTime)
{
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
		Vector3(1100, 480, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	playButton->GetComponent<SpriteRenderer>()->alpha = 0;
	playButton->isActive = false;

	helpButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("help_button"),
		Vector3(1100, 330, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	helpButton->GetComponent<SpriteRenderer>()->alpha = 0;
	helpButton->isActive = false;

	optionsButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("options_button"),
		Vector3(1040, 180, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	optionsButton->GetComponent<SpriteRenderer>()->alpha = 0;
	optionsButton->isActive = false;

	backButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("back_button"),
		Vector3(50, 650, UPPER_GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	backButton->GetComponent<SpriteRenderer>()->alpha = 0;
	backButton->isActive = false;

	calibrateButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("calibrate_button"),
		Vector3(590, 200, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	calibrateButton->GetComponent<SpriteRenderer>()->alpha = 0;
	calibrateButton->isActive = false;

	mascot = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("mascot"),
		Vector3(300, 350, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	mascot->GetComponent<Animation>()->alpha = 0;
	mascot->isActive = false;

	menuBG = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("menu_BG"),
		Vector3(640, 360, BG_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	menuBG->GetComponent<SpriteRenderer>()->alpha = 0;
	menuBG->isActive = false;

	tutorial = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("tutorial"),
		Vector3(640, 360, BG_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	tutorial->GetComponent<SpriteRenderer>()->alpha = 0;
	tutorial->isActive = false;


	gameBG = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("bg"),
		Vector3(640, 360, BG_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	gameBG->GetComponent<Animation>()->alpha = 0;
	gameBG->isActive = false;

	musicPlus = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("plus_button"),
		Vector3(1000, 570, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	musicPlus->GetComponent<SpriteRenderer>()->alpha = 0;
	musicPlus->isActive = false;

	musicMinus = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("minus_button"),
		Vector3(800, 570, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	musicMinus->GetComponent<SpriteRenderer>()->alpha = 0;
	musicMinus->isActive = false;

	

	title = TextManager::GetInstance()->AddText("monogram", "Super Space Rhythm", Vector4(0.776, 0.4039, 0.5529, 1.0), 580, 600, 2, 2);
	title->color.w = 0;
	title->isActive = false;

	victoryText = TextManager::GetInstance()->AddText("monogram", "Victory", Vector4(0.776, 0.4039, 0.5529, 1.0), 300, 300, 1, 1);
	victoryText->color.w = 0;
	victoryText->isActive = false;

	defeatText = TextManager::GetInstance()->AddText("monogram", "Defeat", Vector4(0.776, 0.4039, 0.5529, 1.0), 300, 300, 1, 1);
	defeatText->color.w = 0;
	defeatText->isActive = false;

	musicVolText = TextManager::GetInstance()->AddText("monogram", "Music", Vector4(0.776, 0.4039, 0.5529, 1.0), 350, 550, 3, 3);
	musicVolText->color.w = 0;
	musicVolText->isActive = false;

	musicVolumeHolder = TextManager::GetInstance()->AddText("monogram", _strdup(musicVolume), Vector4(0.776, 0.4039, 0.5529, 1.0), 860, 550, 2, 2);
	musicVolumeHolder->color.w = 0;
	musicVolumeHolder->isActive = false;

	calBeat = TextManager::GetInstance()->AddText("monogram", "Click on screen when you hear the beat or see the signal", Vector4(0.776, 0.4039, 0.5529, 1.0), 150, 400, 1, 1);
	calBeat->color.w = 0;
	calBeat->isActive = false;

	calComplete = TextManager::GetInstance()->AddText("monogram", "Calibration complete", Vector4(0.776, 0.4039, 0.5529, 1.0), 460, 200, 1, 1);
	calComplete->color.w = 0;
	calComplete->isActive = false;

	victoryScreen = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("victory_screen"),
		Vector3(640, 360, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	victoryScreen->GetComponent<SpriteRenderer>()->alpha = 0;
	victoryScreen->isActive = false;

	preOrderButton = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("preOrder_button"),
		Vector3(640, 360, GUI_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	preOrderButton->GetComponent<SpriteRenderer>()->alpha = 0;
	preOrderButton->isActive = false;

	ladder = Spawn("ladder",
		SceneManager::GetInstance()->GetBlueprintByName("ladder"),
		Vector3(640, 360, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	((Ladder*)ladder)->SetCollidable(false);
	ladder->isActive = false;

	player = Spawn("player",
		SceneManager::GetInstance()->GetBlueprintByName("player"),
		Vector3(640, 360, PLAYER_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	((Player*)player)->SetCollidable(false);
	player->isActive = false;

	shopKeeper = Spawn("shopKeeper",
		SceneManager::GetInstance()->GetBlueprintByName("shop_keeper"),
		Vector3(640, 360, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	((ShopKeeper*)shopKeeper)->SetCollidable(false);
	shopKeeper->isActive = false;

	voidShell = Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("void_shell_GUI"),
		Vector3(player->transform->position.x, player->transform->position.y + 75, UPPER_GUI_LAYER),
		Vector3(0.75, 0.75, 1),
		Vector3());
	voidShell->isActive = false;

	float GUIOffset = 50.0f;
	for (int i = 0; i < 5; i++) {
		GameObject* icon = SceneManager::GetInstance()->SpawnGUI(
			SceneManager::GetInstance()->GetBlueprintByName("full_health_GUI"),
			Vector3(50 + i* GUIOffset, Globals::screenHeight - 50, UPPER_GUI_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		healthBar.push_back(icon);
		icon->isActive = false;
	}

	for (int i = 0; i < 5; i++) {
		GameObject* icon = SceneManager::GetInstance()->SpawnGUI(
			SceneManager::GetInstance()->GetBlueprintByName("half_health_GUI"),
			Vector3(50 + i* GUIOffset, Globals::screenHeight - 50, UPPER_GUI_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		halfHealthBar.push_back(icon);
		icon->isActive = false;
	}

	for (int i = 0; i < 5; i++) {
		GameObject* icon = SceneManager::GetInstance()->SpawnGUI(
			SceneManager::GetInstance()->GetBlueprintByName("empty_health_GUI"),
			Vector3(50 + i* GUIOffset, Globals::screenHeight - 50, UPPER_GUI_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		emptyHealthBar.push_back(icon);
		icon->isActive = false;
	}

	float shieldGUIOffset = 30.0f;
	for (int i = 0; i < 3; i++) {
		GameObject* icon = SceneManager::GetInstance()->SpawnGUI(
			SceneManager::GetInstance()->GetBlueprintByName("shield_GUI"),
			Vector3(45 + i* shieldGUIOffset, Globals::screenHeight - 100, UPPER_GUI_LAYER),
			Vector3(0.6, 0.6, 1),
			Vector3());
		shieldBar.push_back(icon);
		icon->isActive = false;
	}

	SetState(&GameManager::SplashScreen);
}

void GameManager::SplashScreen(float deltaTime)
{
	//Transitions
	if (InputManager::GetInstance()->keyMap != 0) {
		//Skip splash screen
		HideAllGUI();
		SetState(&GameManager::Menu);
		return;
	}

	//State Execution
	splashScreen->isActive = true;
	if (counter > 0.5)
		splashScreen->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);

	if (counter > 4.5) {
		splashScreen->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		if (splashScreen->GetComponent<SpriteRenderer>()->alpha == 0) {
			HideAllGUI();
			SetState(&GameManager::Menu);
		}
	}
}

void GameManager::Menu(float deltaTime)
{
	SoundManager::GetInstance()->currentlyPlaying = SoundManager::GetInstance()->getTrack("level");

	SoundManager::GetInstance()->startConductor = TRUE;
	//Transitions
	if (InputManager::GetInstance()->ActionCheck(DebugMode)) {
		InputManager::GetInstance()->keyUpdate('B', false);
		HideAllGUI();
		SetState(&GameManager::LoadDebugRoom);
		return;
	}

	//State Execution
	playButton->isActive = true;
	optionsButton->isActive = true;
	menuBG->isActive = true;
	title->isActive = true;
	helpButton->isActive = true;
	mascot->isActive = true;

	if (counter > 0.5) {
		playButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
		helpButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
		optionsButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
		menuBG->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
		mascot->GetComponent<Animation>()->FadeIn(FADE_SPEED);
		title->FadeIn(1);
	}
	
	if (playButton->GetComponent<Button>()->isClicked && (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3))
		playPressed = true;

	if (optionsButton->GetComponent<Button>()->isClicked && (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3))
		optionPressed = true;

	if (helpButton->GetComponent<Button>()->isClicked && (SoundManager::GetInstance()->signalPass == 2 || SoundManager::GetInstance()->signalPass == 3))
		helpPressed = true;

	if (playPressed) {
		playButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		optionsButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		helpButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);

		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		title->FadeOut(FADE_SPEED);
		mascot->GetComponent<Animation>()->FadeOut(FADE_SPEED);


		if (playButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			playPressed = false;
			HideAllGUI();
			SetState(&GameManager::LoadRoom);
		}
	}

	if (helpPressed) {

		playButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		optionsButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		helpButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);

		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		title->FadeOut(FADE_SPEED);
		mascot->GetComponent<Animation>()->FadeOut(FADE_SPEED);

		if (helpButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			helpPressed = false;
			HideAllGUI();
			SetState(&GameManager::Tutorial);
		}
	}

	if (optionPressed) {

		playButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		optionsButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		helpButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);

		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		title->FadeOut(FADE_SPEED);
		mascot->GetComponent<Animation>()->FadeOut(FADE_SPEED);

		if (optionsButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			optionPressed = false;
			HideAllGUI();
			SetState(&GameManager::Option);
		}
	}
}

void GameManager::VictoryScreen(float deltaTime)
{
	if (counter > 0) {
		victoryScreen->isActive = true;
		victoryScreen->GetComponent<SpriteRenderer>()->FadeIn(0.4);
	}

	if (InputManager::GetInstance()->isMouseDown || InputManager::GetInstance()->keyMap) {
		doneVictory = true;
	}

	if (doneVictory) {
		victoryScreen->GetComponent<SpriteRenderer>()->FadeOut(0.4);
	}

	if (counter > 2 && victoryScreen->GetComponent<SpriteRenderer>()->alpha == 0) {
		doneVictory = false;
		victoryScreen->isActive = false;
		SetState(&GameManager::PreOrder);
	}
}

void GameManager::PreOrder(float deltaTime)
{
	preOrderButton->isActive = true;
	preOrderButton->GetComponent<SpriteRenderer>()->FadeIn(0.5);

	if (preOrderButton->GetComponent<Button>()->isClicked) {
		ShellExecute(NULL, NULL, "https://www.youtube.com/watch?v=dQw4w9WgXcQ", NULL, NULL, SW_SHOWNORMAL);
		preOrderButton->GetComponent<SpriteRenderer>()->alpha = 0;
		preOrderButton->isActive = false;
		SetState(&GameManager::Menu);
	}
}

void GameManager::Tutorial(float deltaTime)
{
	tutorial->isActive = true;
	backButton->isActive = true;

	tutorial->GetComponent<SpriteRenderer>()->FadeIn(1);
	backButton->GetComponent<SpriteRenderer>()->FadeIn(1);


	if (backButton->GetComponent<Button>()->isClicked)
		backPressed = true;

	if (backPressed) {
		tutorial->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		backButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		if (backButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			backPressed = false;
			HideAllGUI();
			SetState(&GameManager::Menu);
		}
	}
}

void GameManager::Option(float deltaTime)
{
	backButton->isActive = true;
	menuBG->isActive = true;
	musicVolText->isActive = true;
	calibrateButton->isActive = true;
	musicPlus->isActive = true;
	musicMinus->isActive = true;
	musicVolumeHolder->isActive = true;




	menuBG->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	backButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	calibrateButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	musicVolText->FadeIn(FADE_SPEED);
	musicVolumeHolder->FadeIn(FADE_SPEED);


	musicPlus->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	musicMinus->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);

	if (backButton->GetComponent<Button>()->isClicked)
		backPressed = true;

	if (calibrateButton->GetComponent<Button>()->isClicked)
		calPressed = true;

	if (musicMinus->GetComponent<Button>()->isClicked)
		musicMinusPressed = true;
	else musicMinusPressed = false;

	if (musicPlus->GetComponent<Button>()->isClicked)
		musicPlusPressed = true;
	else musicPlusPressed = false;

	std::string temp = std::to_string(SoundManager::GetInstance()->musicVolume);
	musicVolume = _strdup(temp.c_str());
	musicVolumeHolder->context = musicVolume;

	if (backPressed) {
		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		backButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		calibrateButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicVolText->FadeOut(FADE_SPEED);
		musicPlus->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicMinus->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicVolumeHolder->FadeOut(FADE_SPEED);
		if (backButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			backPressed = false;
			HideAllGUI();
			SetState(&GameManager::Menu);
		}
	}

	if (calPressed) {
		calibrate = FALSE;
		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		backButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		calibrateButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicVolText->FadeOut(FADE_SPEED);
		musicPlus->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicMinus->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		musicVolumeHolder->FadeOut(FADE_SPEED);
		if (calibrateButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			calPressed = false;
			HideAllGUI();
			SetState(&GameManager::LoadCalibrate);
		}
	}
	//music vol down
	if (musicMinusPressed) {
		if (!isMusicMinusPressed) {
			SoundManager::GetInstance()->musicVolume -= 5;
			musicMinusPressed = false;
			isMusicMinusPressed = true;
		}
	}
	else isMusicMinusPressed = false;

	//music vol up
	if (musicPlusPressed) {
		if (!isMusicPlusPressed) {
			SoundManager::GetInstance()->musicVolume += 5;
			musicPlusPressed = false;
			isMusicPlusPressed = true;
		}
	}
	else isMusicPlusPressed = false;


}

void GameManager::LoadCalibrate(float deltaTime)
{
	if (!calibrate)
	{
		SoundManager::GetInstance()->audioCalibrated = FALSE;
		SoundManager::GetInstance()->visualCalibrated = FALSE;
	}
	SoundManager::GetInstance()->startConductor = FALSE;
	backButton->isActive = true;
	menuBG->isActive = true;
	calBeat->isActive = true;
	calComplete->isActive = true;
	calibrate = TRUE;

	menuBG->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	backButton->GetComponent<SpriteRenderer>()->FadeIn(FADE_SPEED);
	calBeat->FadeIn(FADE_SPEED);


	if (backButton->GetComponent<Button>()->isClicked)
		backPressed = true;
	
	if (backPressed) {
		SoundManager::GetInstance()->counter = 0;
		SoundManager::GetInstance()->startConductor = TRUE;
		menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		backButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		calBeat->FadeOut(FADE_SPEED);
		calComplete->FadeOut(FADE_SPEED);
		if (backButton->GetComponent<SpriteRenderer>()->alpha == 0) {
			backPressed = false;
			HideAllGUI();
			SetState(&GameManager::Option);
		}
	}
		

	if (SoundManager::GetInstance()->audioCalibrated
		&& SoundManager::GetInstance()->visualCalibrated) {
		//menuBG->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		//backButton->GetComponent<SpriteRenderer>()->FadeOut(FADE_SPEED);
		calComplete->FadeIn(FADE_SPEED);
		//HideAllGUI();
		//SetState(&GameManager::Option);
	}
}


void GameManager::LoadDebugRoom(float deltaTime)
{
	room = Spawn("room",
		SceneManager::GetInstance()->GetBlueprintByName("room_2"),
		Vector3(640, 360, ROOM_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	player = Spawn("player",
		SceneManager::GetInstance()->GetBlueprintByName("player"),
		Vector3(640, 100, PLAYER_LAYER),
		Vector3(1, 1, 1),
		Vector3());


	Spawn("object",
		SceneManager::GetInstance()->GetBlueprintByName("bg"),
		Vector3(640, 360, BG_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	//SpawnToRoom("shopKeeper",
	//	SceneManager::GetInstance()->GetBlueprintByName("shop_keeper"),
	//	Vector3(540, 360, PLAYER_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("ladder",
	//	SceneManager::GetInstance()->GetBlueprintByName("ladder"),
	//	Vector3(540, 360, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());

	/*SpawnToRoom("boss",
		SceneManager::GetInstance()->GetBlueprintByName("boss"),
		Vector3(700, 500, PLAYER_LAYER),
		Vector3(1, 1, 1),
		Vector3());*/

	////SpawnToRoom("hp",
	////	SceneManager::GetInstance()->GetBlueprintByName("health_potion"),
	////	Vector3(800, 300, TRAP_LAYER),
	////	Vector3(1, 1, 1),
	////	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_white"),
	//	Vector3(800, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_red"),
	//	Vector3(850, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
	//	Vector3(750, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
	//	Vector3(750, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
	//	Vector3(750, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mob",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_blue"),
	//	Vector3(750, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());

	//SpawnToRoom("mobExplode",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_explode"),
	//	Vector3(600, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mobExplodeBullet",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_explode_bullet"),
	//	Vector3(500, 200, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	SpawnToRoom("mobShoot",
		SceneManager::GetInstance()->GetBlueprintByName("mob_shoot"),
		Vector3(700, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	/*SpawnToRoom("mobKnight",
		SceneManager::GetInstance()->GetBlueprintByName("mob_knight"),
		Vector3(400, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());*/
	//SpawnToRoom("mobWiz",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_wiz"),
	//	Vector3(300, 201, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mobNecro",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_necro"),
	//	Vector3(200, 201, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("mobNecro",
	//	SceneManager::GetInstance()->GetBlueprintByName("mob_necro"),
	//	Vector3(200, 201, MOB_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	/*SpawnToRoom("mobDino",
		SceneManager::GetInstance()->GetBlueprintByName("mob_dino"),
		Vector3(100, 200, MOB_LAYER),
		Vector3(1, 1, 1),
		Vector3());*/
	SpawnToRoom("trap",
		SceneManager::GetInstance()->GetBlueprintByName("trap"),
		Vector3(800, 500, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	//SpawnToRoom("mine",
	//	SceneManager::GetInstance()->GetBlueprintByName("mine"),
	//	Vector3(900, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(500, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(550, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	//SpawnToRoom("crate",
	//	SceneManager::GetInstance()->GetBlueprintByName("crate"),
	//	Vector3(600, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	SpawnToRoom("tnt",
		SceneManager::GetInstance()->GetBlueprintByName("tnt"),
		Vector3(750, 700, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	//SpawnToRoom("bomb",
	//	SceneManager::GetInstance()->GetBlueprintByName("bomb"),
	//	Vector3(800, 700, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	SpawnToRoom("tnt",
		SceneManager::GetInstance()->GetBlueprintByName("tnt"),
		Vector3(800, 700, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("tnt",
		SceneManager::GetInstance()->GetBlueprintByName("tnt"),
		Vector3(850, 700, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("tnt",
		SceneManager::GetInstance()->GetBlueprintByName("tnt"),
		Vector3(900, 700, TRAP_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	//SpawnToRoom("tnt",
	//	SceneManager::GetInstance()->GetBlueprintByName("tnt"),
	//	Vector3(650, 500, TRAP_LAYER),
	//	Vector3(1, 1, 1),
	//	Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("void_shell_buff"),
		Vector3(300, 300, ITEM_LAYER),
		Vector3(1,1,1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("health_buff"),
		Vector3(350, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("shield_buff"),
		Vector3(400, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("magic_beat_buff"),
		Vector3(450, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("side_gun_buff"),
		Vector3(500, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("enhanced_muzzle_buff"),
		Vector3(550, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("blade_mail_buff"),
		Vector3(600, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("invincible_cloak_buff"),
		Vector3(650, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("swift_slipper_buff"),
		Vector3(700, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());
	SpawnToRoom("perk",
		SceneManager::GetInstance()->GetBlueprintByName("enhanced_shell_buff"),
		Vector3(750, 300, ITEM_LAYER),
		Vector3(1, 1, 1),
		Vector3());

	SetState(&GameManager::RoomStart);
}


void GameManager::RoomStart(float deltaTime)
{
	camera->UpdatePosition(player->transform->position.x - (float)Globals::screenWidth / 2, player->transform->position.y - (float)Globals::screenHeight / 2, deltaTime);

	if (InputManager::GetInstance()->ActionCheck(DebugMode)) {
		counter = 0;
		InputManager::GetInstance()->keyUpdate('B', false);
		SetState(&GameManager::Victory);
	}

	if (script) {
		while (scriptIter < script->spawnInfos.size() && counter >= script->spawnInfos.at(scriptIter).time) {
			Vector2 spawnPos;

			float xOffset = rand() % SPAWN_DEPTH + NONE_SPAWNING_RADIUS;
			float yOffset = rand() % SPAWN_DEPTH + NONE_SPAWNING_RADIUS;
			if (rand() % 2 == 0) xOffset = -xOffset;
			if (rand() % 2 == 0) yOffset = -yOffset;
			spawnPos.x = Globals::clamp_x(player->transform->position.x + xOffset);
			spawnPos.y = Globals::clamp_y(player->transform->position.y + yOffset);

			SpawnToRoom(script->spawnInfos.at(scriptIter).constructor,
				SceneManager::GetInstance()->GetBlueprintByName(script->spawnInfos.at(scriptIter).blueprint),
				Vector3(spawnPos.x, spawnPos.y, script->spawnInfos.at(scriptIter).layer),
				Vector3(1,1,1),
				Vector3());

			scriptIter++;
		}
		
		//Ded or win
		if (mobCount == script->clearCount) {
			// Win
			counter = 0;
			SetState(&GameManager::Victory);
		}
		if (player->GetComponent<HP>()->dead) {
			// Dead
			counter = 0;
			SetState(&GameManager::Defeat);
		}
	}
}

void GameManager::Victory(float deltaTime)
{
	camera->UpdatePosition(player->transform->position.x - (float)Globals::screenWidth / 2, player->transform->position.y - (float)Globals::screenHeight / 2, deltaTime);

	//Shopping phase
	if (!shopped) {
		if (!shopSpawned) {
			((ShopKeeper*)shopKeeper)->SetCollidable(true);
			shopKeeper->isActive = true;
			Vector2 spawnPos;

			float xOffset = rand() % SPAWN_DEPTH + NONE_SPAWNING_RADIUS;
			float yOffset = rand() % SPAWN_DEPTH + NONE_SPAWNING_RADIUS;
			if (rand() % 2 == 0) xOffset = -xOffset;
			if (rand() % 2 == 0) yOffset = -yOffset;

			shopKeeper->UpdatePosition(clamp_X(player->transform->position.x + xOffset),  clamp_Y(player->transform->position.y + yOffset), shopKeeper->transform->position.z);

			SpawnBuff();

			shopSpawned = true;
		}

		if (InputManager::GetInstance()->ActionCheck(DebugMode)) {
			InputManager::GetInstance()->keyUpdate('B', false);
			shopped = true;
		}
	}
		
	if (shopped) {
		((ShopKeeper*)shopKeeper)->SetCollidable(false);
		shopKeeper->isActive = false;

		for (std::list<GameObject*>::iterator it = spawnedBuffList.begin(); it != spawnedBuffList.end(); ++it)
			addToRemovalList(*it);
		spawnedBuffList.clear();

		((Ladder*)ladder)->SetCollidable(true);
		ladder->UpdatePosition(shopKeeper->transform->position.x, shopKeeper->transform->position.y, ladder->transform->position.z);
		ladder->isActive = true;
	}

	if (((Player*)player)->activeState == &Player::ClimbLadder) {
		room->GetComponent<SpriteRenderer>()->FadeOut(0.5);
		gameBG->GetComponent<Animation>()->alpha = 0;
		ladder->GetComponent<Animation>()->FadeOut(0.5);
		((Player*)player)->FadeOut(0.5);
	}

	//Move to the next room when ladder animation is finished
	if (room->GetComponent<SpriteRenderer>()->alpha == 0) {
		shopped = false;
		shopSpawned = false;

		ladder->GetComponent<Animation>()->alpha = 1;
		((Ladder*)ladder)->SetCollidable(false);
		ladder->isActive = false;

		((Player*)player)->SetCollidable(false);
		((Player*)player)->alpha = 1;
		player->isActive = false;
		camera->SnapPosition(0, 0);

		SetState(&GameManager::UnloadRoom);
	}
}

void GameManager::Defeat(float deltaTime)
{
	SoundManager::GetInstance()->startConductor = FALSE;

	defeatText->isActive = true;
	defeatText->UpdatePosition(player->transform->position.x - 60, player->transform->position.y + 120);

	if (counter > 1)
		defeatText->FadeIn(0.5);
	if (counter > 3)
		defeatText->FadeOut(0.5);

	if (counter > 3 && defeatText->color.w == 0) {
		defeatText->isActive = false;
		defeatText->color.w = 0;

		addToRemovalList(player);
		player = Spawn("player",
			SceneManager::GetInstance()->GetBlueprintByName("player"),
			Vector3(640, 360, PLAYER_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		((Player*)player)->SetCollidable(false);
		player->isActive = false;
		camera->SnapPosition(0, 0);

		scriptIter = 0;
		roomNum = 0;
		
		ResetRoomAndBuff();

		UnloadObjects();
		SetState(&GameManager::Menu);
	}
}

void GameManager::UnloadRoom(float deltaTime)
{
	UnloadObjects();
	if (roomNum == 1)
	{
		SoundManager::GetInstance()->startConductor = FALSE;
	}
	scriptIter = 0;
	roomNum++;
	SetState(&GameManager::LoadRoom);
}

void GameManager::LoadRoom(float deltaTime)
{
	if (roomNum >= 3) {
		SoundManager::GetInstance()->startConductor = FALSE;
		ResetRoomAndBuff();
		HideHealthBar();
		counter = 0;
		SetState(&GameManager::VictoryScreen);
		return;
	}

	if (roomNum == 2)
	{
		SoundManager::GetInstance()->currentlyPlaying = SoundManager::GetInstance()->getTrack("boss");
		SoundManager::GetInstance()->startConductor = TRUE;

		room = room = SpawnToRoom("room",
			SceneManager::GetInstance()->GetBlueprintByName(bossRoom),
			Vector3(640, 360, ROOM_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		room->name = _strdup("room");

		script = SceneManager::GetInstance()->GetScriptByName(bossRoom);
	}
	else {
		char* usingRoom;
		while (1) {
			int randomRoom = rand() % 8;

			//Check if player already cleared this room before
			if (roomList[randomRoom].timeUsed > 0)
				continue;

			usingRoom = roomList[randomRoom].roomName;
			roomList[randomRoom].timeUsed++;
			break;
		}

		room = SpawnToRoom("room",
			SceneManager::GetInstance()->GetBlueprintByName(usingRoom),
			Vector3(640, 360, ROOM_LAYER),
			Vector3(1, 1, 1),
			Vector3());
		room->name = _strdup("room");

		script = SceneManager::GetInstance()->GetScriptByName(usingRoom);
	}

	((Player*)player)->SetCollidable(true);
	player->isActive = true;
	player->UpdatePosition(640, 360, PLAYER_LAYER);

	gameBG->isActive = true;
	gameBG->GetComponent<Animation>()->alpha = 1;

	counter = 0;
	mobCount = 0;
	SetState(&GameManager::RoomStart);
}

void GameManager::SpawnBuff()
{
	//Type of buff
	//TODO Remove hardcode
	int spawnedBuff[3];
	for (int i = 0; i < 3; i++) {
		while (1) {
			int buff = rand() % 9;

			bool skip = false;

			//Check if player already recieved this buff
			if (perkList.at(buff)->timeUsed > 0)
				continue;

			//Check if its already one of the 3/4 buffs
			for (int j = 0; j < i; j++) {
				if (spawnedBuff[j] == buff) {
					skip = true;
					break;
				}
			}
			if (skip) continue;

			spawnedBuff[i] = buff;
			break;
		}
	}

	for (int i = 0; i < 3; i++) {
		Vector2 spawnPos;

		spawnPos.x = shopKeeper->transform->position.x - 100 + buffOffset*i;
		spawnPos.y = shopKeeper->transform->position.y - 100;

		spawnedBuffList.push_back(SpawnToRoom("perk",
			perkList.at(spawnedBuff[i])->blueprint,
			Vector3(spawnPos.x, spawnPos.y, ITEM_LAYER),
			Vector3(1,1,1),
			Vector3()));
	}
}

//Reset all objects to pos-Init() state
void GameManager::HideAllGUI()
{
	counter = 0;

	splashScreen->GetComponent<SpriteRenderer>()->alpha = 0;
	splashScreen->GetComponent<SpriteRenderer>()->FadeOff();
	splashScreen->isActive = false;

	playButton->GetComponent<SpriteRenderer>()->alpha = 0;
	playButton->GetComponent<SpriteRenderer>()->FadeOff();
	playButton->isActive = false;


	helpButton->GetComponent<SpriteRenderer>()->alpha = 0;
	helpButton->GetComponent<SpriteRenderer>()->FadeOff();
	helpButton->isActive = false;

	optionsButton->GetComponent<SpriteRenderer>()->alpha = 0;
	optionsButton->GetComponent<SpriteRenderer>()->FadeOff();
	optionsButton->isActive = false;

	menuBG->GetComponent<SpriteRenderer>()->alpha = 0;
	menuBG->GetComponent<SpriteRenderer>()->FadeOff();
	menuBG->isActive = false;


	mascot->GetComponent<Animation>()->alpha = 0;
	mascot->GetComponent<Animation>()->FadeOff();
	mascot->isActive = false;

	title->color.w = 0;
	title->FadeOff();
	title->isActive = false;

	tutorial->GetComponent<SpriteRenderer>()->alpha = 0;
	tutorial->GetComponent<SpriteRenderer>()->FadeOff();
	tutorial->isActive = false;

	backButton->GetComponent<SpriteRenderer>()->alpha = 0;
	backButton->GetComponent<SpriteRenderer>()->FadeOff();
	backButton->isActive = false;

	musicPlus->GetComponent<SpriteRenderer>()->alpha = 0;
	musicPlus->GetComponent<SpriteRenderer>()->FadeOff();
	musicPlus->isActive = false;

	musicMinus->GetComponent<SpriteRenderer>()->alpha = 0;
	musicMinus->GetComponent<SpriteRenderer>()->FadeOff();
	musicMinus->isActive = false;

	calibrateButton->GetComponent<SpriteRenderer>()->alpha = 0;
	calibrateButton->GetComponent<SpriteRenderer>()->FadeOff();
	calibrateButton->isActive = false;


	musicVolText->color.w = 0;
	musicVolText->FadeOff();
	musicVolText->isActive = false;

	calBeat->color.w = 0;
	calBeat->FadeOff();
	calBeat->isActive = false;

	calComplete->color.w = 0;
	calComplete->FadeOff();
	calComplete->isActive = false;

	musicVolumeHolder->color.w = 0;
	musicVolumeHolder->FadeOff();
	musicVolumeHolder->isActive = false;
}

void GameManager::HideHealthBar()
{
	for (int i = 0; i < 5; i++) {
		healthBar.at(i)->isActive = false;
		halfHealthBar.at(i)->isActive = false;
		emptyHealthBar.at(i)->isActive = false;
	}
	voidShell->isActive = false;
	for (int i = 0; i < 3; i++) {
		shieldBar.at(i)->isActive = false;
	}
}

float GameManager::clamp_X(float x)
{
	if (player->transform->position.x + x > X_MAX - 50 || player->transform->position.x + x < X_MIN + 50)
		return Globals::clamp_x(-x);
	return Globals::clamp_x(x);
}

float GameManager::clamp_Y(float y)
{
	if (player->transform->position.y + y > Y_MAX - 50 || player->transform->position.y + y < Y_MIN + 50)
		return Globals::clamp_y(-y);
	return Globals::clamp_y(y);
}
