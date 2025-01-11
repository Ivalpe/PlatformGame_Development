#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Power.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlSlider.h"


// -----------------------------
// Constructor and Destructor
// -----------------------------
Scene::Scene() : Module()
{
	name = "scene";
	img = nullptr;
}
Scene::~Scene()
{}

// -----------------------------
// Initialization and Setup
// -----------------------------

// Called before render is available
bool Scene::Awake()
{
	alpha = 0;
	fadeIn = false;
	LOG("Loading Scene");
	bool ret = true;

	exitGame = false;
	firstTimeLoad = false;
	help = false;
	colRespawn = 120;
	level = 0;
	idBonfire = 1;
	idNameBonfire = 1;

	//Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));
	player->DisablePlayer();

	coordYMenuTp = 350;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	LoadAssets();

	int levelNumber = configParameters.child("levels").child("map").attribute("number").as_int();

	// Play different music based on the level number
	switch (levelNumber)
	{
	case 0:
		Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music1SFX").attribute("path").as_string(), 0.7f);
		break;
	case 1:
		Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music2SFX").attribute("path").as_string(), 0.7f);
		break;
	case 2:
		Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music3SFX").attribute("path").as_string(), 0.7f);
		break;
	case 3:
		Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music4SFX").attribute("path").as_string(), 0.7f);
		break;
	default:
		// Optional: handle cases where the level number is out of range
		break;
	}

	//Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", configParameters.child("levels").child("map").attribute("name").as_string());
	RestartEnemies();
	CreateEvents();

	SetupUI();

	return true;
}

// -----------------------------
// Game Loop Functions
// -----------------------------

void Scene::LoadAssets() {
	// Texture to highligh mouse position 
	mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");
	gui = Engine::GetInstance().textures.get()->Load("Assets/Textures/hud.png");
	lifePlayer = Engine::GetInstance().textures.get()->Load("Assets/Textures/life.png");
	sliderBackground = Engine::GetInstance().textures.get()->Load("Assets/Textures/slider1.png");
	sliderMovement = Engine::GetInstance().textures.get()->Load("Assets/Textures/slider2.png");
	menuButtonNormal = Engine::GetInstance().textures.get()->Load("Assets/Menus/button.png");
	menuButtonFocused = Engine::GetInstance().textures.get()->Load("Assets/Menus/buttonfocus.png");
	menuButtonPressed = Engine::GetInstance().textures.get()->Load("Assets/Menus/buttonPressed.png");
	helpMenu = Engine::GetInstance().textures.get()->Load("Assets/Textures/HelpMenu.png");
	powerOff = Engine::GetInstance().textures.get()->Load("Assets/Textures/powerOff.png");
	powerOn = Engine::GetInstance().textures.get()->Load("Assets/Textures/powerOn.png");
	pouch = Engine::GetInstance().textures.get()->Load("Assets/Textures/pouch.png");
	pouchfull = Engine::GetInstance().textures.get()->Load("Assets/Textures/pouchfull.png");

	bonfireSFX = Engine::GetInstance().audio.get()->LoadFx(configParameters.child("audio").child("fx").child("bonfireSFX").attribute("path").as_string());
	loadSFX = Engine::GetInstance().audio.get()->LoadFx(configParameters.child("audio").child("fx").child("loadsSFX").attribute("path").as_string());
	saveSFX = Engine::GetInstance().audio.get()->LoadFx(configParameters.child("audio").child("fx").child("saveSFX").attribute("path").as_string());

	OptionsBook = Engine::GetInstance().textures.get()->Load("Assets/Menus/OptionsBook.png");
	TitleScreen = Engine::GetInstance().textures.get()->Load("Assets/Menus/TitleScreen.png");
	Engine::GetInstance().uiManager.get()->LoadTextures(TitleScreen, OptionsBook);
}

void Scene::SetupUI() {
	//Main Menu
	std::vector<const char*> names = { "    New Game", "    Load Game", "    Settings", "    Credits" , "    Exit Game" };
	int coordInitial = 360, interspace = 70;
	GuiControlButton* button;
	for (auto n : names) {
		button = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, Engine::GetInstance().uiManager.get()->GetSize(GuiClass::MAIN_MENU), n, { 480, coordInitial, 200,60 }, this, GuiClass::MAIN_MENU);
		button->SetTexture(menuButtonNormal, menuButtonFocused, menuButtonPressed);
		Engine::GetInstance().uiManager.get()->Add(GuiClass::MAIN_MENU, button);
		coordInitial += interspace;
	}
	Engine::GetInstance().uiManager.get()->Active(GuiClass::MAIN_MENU);

	//Settings
	GuiControlSlider* slider = (GuiControlSlider*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, Engine::GetInstance().uiManager.get()->GetSize(GuiClass::SETTINGS), "Music", { 520 / 2, 200, 104,20 }, this, GuiClass::SETTINGS);
	slider->SetTexture(sliderBackground, sliderMovement);
	Engine::GetInstance().uiManager.get()->Add(GuiClass::SETTINGS, slider);
	Engine::GetInstance().uiManager.get()->Disable(GuiClass::SETTINGS);
	showSettings = false;

	//Pause menu
	names = { "      Resume", "     Settings", "      Back To Title", "     Exit" };
	coordInitial = 240, interspace = 70;
	for (auto n : names) {
		button = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, Engine::GetInstance().uiManager.get()->GetSize(GuiClass::PAUSE), n, { 480, coordInitial, 180,60 }, this, GuiClass::PAUSE);
		button->SetTexture(menuButtonNormal, menuButtonFocused, menuButtonPressed);
		Engine::GetInstance().uiManager.get()->Add(GuiClass::PAUSE, button);
		coordInitial += interspace;
	}
	Engine::GetInstance().uiManager.get()->Disable(GuiClass::PAUSE);
	Engine::GetInstance().uiManager.get()->Disable(GuiClass::TPBONFIRE);
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

void Scene::HandleCamera(Engine& engine) {
	if (level == 0) {
		if (!showSettings) {
			SDL_Rect rec;
			rec.x = 0;
			rec.y = 0;
			rec.w = 1500;
			rec.h = 800;
			engine.render.get()->DrawRectangle(rec, 0, 0, 0, alpha, true, false);

			int cameraX = engine.render.get()->camera.x -= 2;
			int cameraMaxX = engine.map.get()->GetWidth() * 8 * -1 + (10 * 8);
			if (cameraX <= cameraMaxX) {
				engine.render.get()->camera.x = cameraMaxX;
				if (!fadeIn) {
					if (alpha < 255) alpha += 5;
					if (alpha >= 255) alpha = 255;
				}

				if (!fadeIn && alpha == 255) {
					fadeIn = true;
					engine.render.get()->camera.x = 0;
				}
			}

			if (fadeIn) {
				if (alpha > 0) alpha -= 5;
				if (alpha <= 0) alpha = 0;
			}

			if (fadeIn && alpha == 0) {
				fadeIn&& alpha == 0;
				fadeIn = false;
			}
		}
	}
	else {
		engine.render.get()->camera.x = ((player->GetX() * -1) + 200) * 2;
		int cameraX = engine.render.get()->camera.x;
		int cameraMaxX = engine.map.get()->GetWidth() * 8 * -1 - (240 * 8);
		if (cameraX >= 0) engine.render.get()->camera.x = 0;
		if (cameraX <= cameraMaxX) engine.render.get()->camera.x = cameraMaxX;
	}
}

void Scene::HandlePowers() {
	//Power Player Fireball
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

		Power* power = (Power*)Engine::GetInstance().entityManager->CreateEntity(EntityType::FIREBALLPLAYER);
		power->SetParameters(configParameters.child("entities").child("fireball"), TypePower::FIREBALL);
		if (player->GetDirection() == DirectionPlayer::LEFT) power->Start(true);
		else power->Start(false);

		Vector2D playerPos = player->GetPosition();
		if (player->GetDirection() == DirectionPlayer::LEFT) power->SetPosition({ playerPos.getX() - 4, playerPos.getY() + 14 });
		else power->SetPosition({ playerPos.getX() + 32, playerPos.getY() + 14 });

		fireballList.push_back(power);
	}

	if (player->GetfirePower() && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {

		Power* bigPower = (Power*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BIGFIREBALLPLAYER);
		bigPower->SetParameters(configParameters.child("entities").child("bigfireball"), TypePower::BIGFIREBALL);
		if (player->GetDirection() == DirectionPlayer::LEFT) bigPower->Start(true);
		else bigPower->Start(false);

		Vector2D playerPos = player->GetPosition();
		if (player->GetDirection() == DirectionPlayer::LEFT) bigPower->SetPosition({ playerPos.getX() - 4, playerPos.getY() + 14 });
		else bigPower->SetPosition({ playerPos.getX() + 32, playerPos.getY() + 14 });

		fireballList.push_back(bigPower);

		// Play the sound effect for the big fireball
		//Engine::GetInstance().audio.get()->PlayFx(bigFireballSFX);

		// Reset the firePower flag
		player->SetfirePower(false);
	}

	//Power Boss Fireball
	for (auto& e : enemyList) {
		if (e->GetType() == EnemyType::BOSS && e->GetBossFireball()) {
			Power* power = (Power*)Engine::GetInstance().entityManager->CreateEntity(EntityType::MELEEATTACK);
			power->SetParameters(configParameters.child("entities").child("meleeattack"), TypePower::MELEEATTACKBOSS);
			power->Start(e->GetDirection() == DirectionEnemy::LEFT);

			Vector2D enemyPos = e->GetPosition();
			if (e->GetDirection() == DirectionEnemy::LEFT) power->SetPosition({ enemyPos.getX() - 20, enemyPos.getY() - 10 });
			else power->SetPosition({ enemyPos.getX() + 20, enemyPos.getY() - 10 });

			fireballList.push_back(power);
			e->SetBossFireball(false);
			break;
		}

	}
}

void Scene::HandleGui() {
	auto& engine = Engine::GetInstance();

	if (level != 0) {
		//Open Help
		if (help) engine.render.get()->DrawTexture(helpMenu, SDL_FLIP_NONE, -engine.render.get()->camera.x / 2, 100);

		//Gui
		engine.render.get()->DrawTexture(gui, SDL_FLIP_NONE, -(engine.render.get()->camera.x / 2) + 10, 10);

		if (player->GetLifes() >= 0) {
			int coinCount = player->GetCoins();
			std::string coinText = "Coins: " + std::to_string(coinCount);
			int coordX = -(engine.render.get()->camera.x / 2) + 32;

			for (size_t i = 0; i < player->GetLifes() + 1; i++) {
				engine.render.get()->DrawTexture(lifePlayer, SDL_FLIP_NONE, coordX, 14);
				coordX += 8;
			}

			int fireIconX = coordX + 20;
			if (player->GetfirePower()) engine.render.get()->DrawTexture(powerOn, SDL_FLIP_NONE, fireIconX, 10);
			else engine.render.get()->DrawTexture(powerOff, SDL_FLIP_NONE, fireIconX, 10);

			if (player->GetCoins() > 0) {
				Engine::GetInstance().render->DrawText(coinText.c_str(), 60, 60, 80, 44);
				engine.render.get()->DrawTexture(pouchfull, SDL_FLIP_NONE, -(engine.render.get()->camera.x / 2) + 10, 30);
			}
			else {
				Engine::GetInstance().render->DrawText(coinText.c_str(), 60, 60, 80, 44);
				engine.render.get()->DrawTexture(pouch, SDL_FLIP_NONE, -(engine.render.get()->camera.x / 2) + 10, 30);
			}

		}

		if (showTp && !pause) Engine::GetInstance().uiManager->Active(GuiClass::TPBONFIRE);
		else Engine::GetInstance().uiManager->Disable(GuiClass::TPBONFIRE);

		//Enable Settings UI
		if (engine.input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
			if (Engine::GetInstance().uiManager->IsActive(GuiClass::PAUSE) || Engine::GetInstance().uiManager->IsActive(GuiClass::SETTINGS)) {
				pause = false;
				Engine::GetInstance().uiManager->Disable(GuiClass::PAUSE);
				Engine::GetInstance().uiManager->Disable(GuiClass::SETTINGS);
			}
			else {
				pause = true;
				Engine::GetInstance().uiManager->Active(GuiClass::PAUSE);
			}
		}
	}
	else {
		if (engine.input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {

			if (Engine::GetInstance().uiManager->IsActive(GuiClass::SETTINGS)) {
				Engine::GetInstance().uiManager->Active(GuiClass::MAIN_MENU);
				showSettings = false;
				Engine::GetInstance().uiManager->Disable(GuiClass::SETTINGS);
			}
		}
	}
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	auto& engine = Engine::GetInstance();
	HandleCamera(engine);

	if (level != 0) {
		//Debug Mode
		DebugMode();

		// Shoot
		HandlePowers();

		// Destroy fireballs collided
		for (auto it = fireballList.begin(); it != fireballList.end(); ) {
			if ((*it)->HasCollision()) {
				engine.physics->DeleteBody((*it)->getBody());
				engine.entityManager->DestroyEntity(*it);
				it = fireballList.erase(it);
			}
			else ++it;
		}

		showTp = false;
		pugi::xml_document saveFile;
		pugi::xml_parse_result result = saveFile.load_file("config.xml");

		// Active bonfire if player touch it
		for (auto& bonfire : bonfireList) {
			if (player->GetPosition().getX() >= bonfire->GetPosition().getX() - 16 && player->GetPosition().getX() <= bonfire->GetPosition().getX() + 8 &&
				player->GetPosition().getY() >= bonfire->GetPosition().getY() - 16 && player->GetPosition().getY() <= bonfire->GetPosition().getY() + 8) {

				int posXBonfire = bonfire->GetPosition().getX();
				pugi::xml_node bonfires = saveFile.child("config").child("scene").child("bonfires").find_child_by_attribute("x", std::to_string(posXBonfire).c_str());
				showTp = true;
				if (!bonfires.attribute("activated").as_bool()) {
					bonfire->ActiveBonfire();
					engine.audio.get()->PlayFx(bonfireSFX);

					bonfires.attribute("activated").set_value("true");
					bonfires.append_attribute("id").set_value(idNameBonfire++);

					GuiControlButton* button = (GuiControlButton*)engine.guiManager->CreateGuiControl(GuiControlType::BUTTON, Engine::GetInstance().uiManager.get()->GetSize(GuiClass::TPBONFIRE), bonfires.attribute("name").as_string(), { 520, coordYMenuTp += 40, 180,60 }, this, GuiClass::TPBONFIRE);
					button->SetTexture(menuButtonNormal, menuButtonFocused, menuButtonPressed);
					Engine::GetInstance().uiManager.get()->Add(GuiClass::TPBONFIRE, button);
				}

				saveFile.child("config").child("scene").child("entities").child("player").attribute("x").set_value(bonfire->GetPosition().getX());
				saveFile.child("config").child("scene").child("entities").child("player").attribute("y").set_value(bonfire->GetPosition().getY());
				saveFile.save_file("config.xml");
			}
		}

		//When player dies, dont move the hitbox
		if (player->GetState() == StatePlayer::DIE) {
			player->pbody->body->SetLinearVelocity({ 0,0 });
			colRespawn--;
		}

		if (colRespawn <= 0) {
			player->Respawn();
			LoadState(LOAD::RESPAWN);
			colRespawn = 120;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (exitGame) ret = false;

	if (!bossActive && level == 3 && player->GetX() >= 470) {
		for (auto e : enemyList) {
			if (e->GetType() == EnemyType::BOSS) {
				e->ActiveBoss();
				bossActive = true;
				break;
			}
		}
	}

	//Clear dead enemies
	for (auto it = enemyList.begin(); it != enemyList.end(); ) {
		if ((*it)->IsDead()) {
			SaveKillEnemy((*it)->GetId());
			Engine::GetInstance().physics->DeleteBody((*it)->getBody());
			Engine::GetInstance().physics->DeleteBody((*it)->getSensorBody());
			Engine::GetInstance().entityManager->DestroyEntity(*it);
			it = enemyList.erase(it);
		}
		else ++it;
	}

	//Clear Collected items
	for (auto it = itemList.begin(); it != itemList.end(); ) {
		if ((*it)->IsCollected()) {
			SaveCollectedItem((*it)->GetId());
			Engine::GetInstance().physics->DeleteBody((*it)->getBody());
			Engine::GetInstance().entityManager->DestroyEntity(*it);
			it = itemList.erase(it);
		}
		else ++it;
	}

	//Next Level
	if (player->GetLevel() == Level::NEXTLVL) {
		level++;
		pugi::xml_node mapNode = configParameters.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());
		Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
		CreateEvents();

		Vector2D posPlayer;
		posPlayer.setX(mapNode.attribute("ix").as_int());
		posPlayer.setY(mapNode.attribute("iy").as_int() - 16);

		player->SetPosition(posPlayer);
		Engine::GetInstance().uiManager.get()->Disable(GuiClass::MAIN_MENU);

		player->SetLevel(Level::DISABLED);


		std::string musicPath;

		//Music in levels
		switch (level) {
		case 0:
			Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music1SFX").attribute("path").as_string(), 0.7f);
			break;
		case 1:
			Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music2SFX").attribute("path").as_string(), 0.7f);
			break;
		case 2:
			Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music2SFX").attribute("path").as_string(), 0.7f);
			break;
		case 3:
			Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music3SFX").attribute("path").as_string(), 0.7f);
			break;
		default:
			break;
		}

		// Play the music if the path is valid
		if (!musicPath.empty()) {
			Engine::GetInstance().audio->PlayMusic(musicPath.c_str(), 0.7f);
		}
	}

	HandleGui();

	return ret;
}

// -----------------------------
// Debug
// -----------------------------

void Scene::DebugMode() {

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN ||
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

		int previousLevel = level;

		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) level = 1;
		else if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) level = 3;


		pugi::xml_node mapNode = configParameters.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());
		if (level != previousLevel) {
			Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
		}
		LoadState(LOAD::INITIAL);
		RemoveLevelEnemies(level);
		CreateEvents();

		player->SetLevel(Level::DISABLED);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		LoadState(LOAD::INITIAL);
		player->Respawn();
		CreateEvents();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) player->ChangeDebug();

	//Limit FPS
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) Engine::GetInstance().LimitFPS();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		if (help) help = false; else help = true;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		SaveState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		LoadState(LOAD::DEBUG);
		CreateEvents();
	}

	//MOUSE TILE
	if (Engine::GetInstance().physics.get()->GetDebug()) {
		//Get mouse position and obtain the map coordinate
		int scale = Engine::GetInstance().window.get()->GetScale();
		Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
		Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(mousePos.getX() - Engine::GetInstance().render.get()->camera.x / scale,
			mousePos.getY() - Engine::GetInstance().render.get()->camera.y / scale);

		//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
		Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(), mouseTile.getY());
		SDL_Rect rect = { 0,0,8,8 };
		Engine::GetInstance().render.get()->DrawTexture(mouseTileTex, SDL_FLIP_NONE,
			highlightTile.getX(),
			highlightTile.getY(),
			&rect);

		// saves the tile pos for debugging purposes
		if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once) {
			tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
			once = true;
		}
	}

}

void Scene::ActiveBonfires() {
	for (auto bon : bonfireList) {
		int posXBonfire = bon->GetPosition().getX();
		pugi::xml_node bonfires = configParameters.child("   bonfires").find_child_by_attribute("x", std::to_string(posXBonfire).c_str());

		if (bonfires.attribute("activated").as_bool() == true) {
			bon->ActiveBonfire();
		}

	}
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);
	pugi::xml_document bonfireParameters;
	pugi::xml_parse_result result = bonfireParameters.load_file("config.xml");
	pugi::xml_node nodes = bonfireParameters.child("config").child("scene");

	switch (control->GetType())
	{
	case GuiClass::MAIN_MENU:
		if (control->id == 1) {
			level++;
			pugi::xml_node mapNode = configParameters.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());

			Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
			CreateEvents();

			Vector2D posPlayer;
			posPlayer.setX(mapNode.attribute("ix").as_int());
			posPlayer.setY(mapNode.attribute("iy").as_int() - 16);

			player->ActivePlayer();
			Engine::GetInstance().uiManager.get()->Disable(GuiClass::MAIN_MENU);
			player->SetPosition(posPlayer);


			player->SetLevel(Level::DISABLED);
		}
		else if (control->id == 2) {
		}
		else if (control->id == 3) {
			if (Engine::GetInstance().uiManager.get()->IsActive(GuiClass::SETTINGS)) {
				Engine::GetInstance().uiManager.get()->Disable(GuiClass::SETTINGS);
				showSettings = false;
			}
			else {
				Engine::GetInstance().uiManager.get()->Disable(GuiClass::MAIN_MENU);
				Engine::GetInstance().uiManager.get()->Active(GuiClass::SETTINGS);
				showSettings = true;
			}
		}
		else if (control->id == 4) {
		}
		else if (control->id == 5) {
			exitGame = true;
		}
		break;
	case GuiClass::PAUSE:
		if (control->id == 1) {
			Engine::GetInstance().uiManager.get()->Disable(GuiClass::PAUSE);
			Engine::GetInstance().uiManager.get()->Disable(GuiClass::SETTINGS);
			pause = false;
		}
		else if (control->id == 2) {
			Engine::GetInstance().uiManager.get()->Active(GuiClass::SETTINGS);
			Engine::GetInstance().uiManager.get()->Disable(GuiClass::PAUSE);
		}
		else if (control->id == 3) {
			level = 0;
			pugi::xml_node mapNode = configParameters.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());

			Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
			CreateEvents();

			pugi::xml_node currentLevel = nodes.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());
			Vector2D posPlayer;
			posPlayer.setX(currentLevel.attribute("ix").as_int());
			posPlayer.setY(currentLevel.attribute("iy").as_int() - 16);

			player->DisablePlayer();
			Engine::GetInstance().uiManager.get()->Active(GuiClass::MAIN_MENU);
			Engine::GetInstance().uiManager.get()->Disable(GuiClass::PAUSE);
			player->SetPosition(posPlayer);
			pause = false;


			player->SetLevel(Level::DISABLED);
		}
		else if (control->id == 4) exitGame = true;
		break;
	case GuiClass::TPBONFIRE:
		pugi::xml_node bonfireNode = nodes.child("bonfires").find_child_by_attribute("id", std::to_string(control->id).c_str());
		if (bonfireNode.attribute("id").as_int() == control->id) {

			if (bonfireNode.attribute("level").as_int() != level) {
				level = bonfireNode.attribute("level").as_int();
				pugi::xml_node mapNode = configParameters.child("levels").find_child_by_attribute("number", std::to_string(level).c_str());
				Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				LoadState(LOAD::RESPAWN);
				CreateEvents();
				ActiveBonfires();
				player->SetLevel(Level::DISABLED);

			}
			player->SetPosition({ bonfireNode.attribute("x").as_float(), bonfireNode.attribute("y").as_float() });
		}

		break;

	}


	return true;
}

// -----------------------------
// State Management
// -----------------------------

void Scene::LoadState(LOAD load) {
	Engine::GetInstance().render.get()->camera.x = 0;

	pugi::xml_document playerParameters;
	pugi::xml_parse_result result = playerParameters.load_file("config.xml");
	pugi::xml_node playerNode = playerParameters.child("config").child("scene").child("entities").child("player");
	pugi::xml_node mapNode = playerParameters.child("config").child("scene").child("levels").find_child_by_attribute("number", std::to_string(level).c_str());

	Vector2D posPlayer;
	if (load == LOAD::INITIAL) {
		posPlayer.setX(mapNode.attribute("ix").as_int());
		posPlayer.setY(mapNode.attribute("iy").as_int());
		RestartEnemies();
	}
	else if (load == LOAD::RESPAWN) {
		posPlayer.setX(playerNode.attribute("x").as_int());
		posPlayer.setY(playerNode.attribute("y").as_int() - 16);
	}
	else {
		int lvlSave = playerNode.attribute("dlevel").as_int();
		if (lvlSave != level) {
			Engine::GetInstance().map->Load("Assets/Maps/", configParameters.child("levels").find_child_by_attribute("number", std::to_string(lvlSave).c_str()).attribute("name").as_string());
			level = lvlSave;
		}
		posPlayer.setX(playerNode.attribute("dx").as_int());
		posPlayer.setY(playerNode.attribute("dy").as_int() - 16);

	}

	player->SetPosition(posPlayer);

	Engine::GetInstance().audio.get()->PlayFx(loadSFX);
}

void Scene::SaveState() {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	saveFile.child("config").child("scene").child("entities").child("player").attribute("dx").set_value(player->GetX());
	saveFile.child("config").child("scene").child("entities").child("player").attribute("dy").set_value(player->GetY());
	saveFile.child("config").child("scene").child("entities").child("player").attribute("dlevel").set_value(level);
	saveFile.save_file("config.xml");

	Engine::GetInstance().audio.get()->PlayFx(saveSFX);
}

// -----------------------------
// Entity Creation and Management
// -----------------------------

void Scene::CreateEvents() {
	std::list<Vector2D> listBonfires;
	std::map<Vector2D, int> listEnemy, listItems;
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	//Firecamps
	for (int i = 0; i < bonfireList.size();) {
		Engine::GetInstance().entityManager->DestroyEntity(bonfireList[i]);
		bonfireList.erase(bonfireList.begin());
	}

	//Enemies
	for (int i = 0; i < enemyList.size();) {
		Engine::GetInstance().physics->DeleteBody(enemyList[i]->getBody());
		Engine::GetInstance().physics->DeleteBody(enemyList[i]->getSensorBody());
		Engine::GetInstance().entityManager->DestroyEntity(enemyList[i]);
		enemyList.erase(enemyList.begin() + i);
	}

	//Items
	for (int i = 0; i < itemList.size(); i++) {
		Engine::GetInstance().physics->DeleteBody(itemList[i]->getBody());
		Engine::GetInstance().entityManager->DestroyEntity(itemList[i]);
		itemList.erase(itemList.begin() + i);
		i--;
	}


	if (!firstTimeLoad) {
		saveFile.child("config").child("scene").child("bonfires").remove_children();
		saveFile.child("config").child("scene").child("enemies").remove_children();
		saveFile.child("config").child("scene").child("items").remove_children();
		saveFile.save_file("config.xml");
		firstTimeLoad = true;
	}

	// BONFIRES

	bool contains = false;
	for (int i = 0; i < levelsLoadedBonfire.size() && !contains; i++) {
		if (levelsLoadedBonfire[i] == level) contains = true;
	}

	listBonfires = Engine::GetInstance().map->GetBonfireList();
	for (auto bonfire : listBonfires) {
		Bonfire* fc = (Bonfire*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BONFIRE);
		fc->SetParameters(configParameters.child("entities").child("firecamp"));
		fc->SetPosition({ bonfire.getX(), bonfire.getY() });
		fc->Start();
		bonfireList.push_back(fc);

		if (!contains) {
			pugi::xml_node new_bonfire = saveFile.child("config").child("scene").child("bonfires").append_child("bonfire");
			new_bonfire.append_attribute("level").set_value(level);
			new_bonfire.append_attribute("activated").set_value("false");
			new_bonfire.append_attribute("x").set_value(bonfire.getX());
			new_bonfire.append_attribute("y").set_value(bonfire.getY());

			name = "Hoguera ";
			name = name + std::to_string(idBonfire++);

			new_bonfire.append_attribute("name").set_value(name.c_str());
			saveFile.save_file("config.xml");
		}

	}
	levelsLoadedBonfire.push_back(level);

	//ENEMIES

	contains = false;
	for (int i = 0; i < levelsLoadedEnemies.size() && !contains; i++) {
		if (levelsLoadedEnemies[i] == level) contains = true;
	}

	listEnemy = Engine::GetInstance().map->GetEnemyList();
	for (auto enemy : listEnemy) {
		int lowestId = GetLowestId(1);
		Enemy* en = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
		if (enemy.second == 1) {
			en->SetParameters(configParameters.child("entities").child("enemies").child("evilwizard"), lowestId);
			en->SetEnemyType(EnemyType::EV_WIZARD);
		}
		else if (enemy.second == 2) {
			en->SetParameters(configParameters.child("entities").child("enemies").child("bat"), lowestId);
			en->SetEnemyType(EnemyType::BAT);
		}
		else {
			en->SetParameters(configParameters.child("entities").child("enemies").child("boss"), lowestId);
			en->SetEnemyType(EnemyType::BOSS);
		}
		en->Start();
		en->SetPosition({ enemy.first.getX(), enemy.first.getY() });
		enemyList.push_back(en);

		if (!contains) {
			pugi::xml_node newEnemy = saveFile.child("config").child("scene").child("enemies").append_child("enemy");
			newEnemy.append_attribute("id").set_value(lowestId);
			newEnemy.append_attribute("level").set_value(level);
			newEnemy.append_attribute("dead").set_value("false");
			newEnemy.append_attribute("x").set_value(enemy.first.getX());
			newEnemy.append_attribute("y").set_value(enemy.first.getY());

			saveFile.save_file("config.xml");
		}

	}

	levelsLoadedEnemies.push_back(level);

	//ITEMS

	contains = false;
	for (int i = 0; i < levelsLoadedItems.size() && !contains; i++) {
		if (levelsLoadedItems[i] == level) contains = true;
	}

	listItems = Engine::GetInstance().map->GetItemList();
	for (auto item : listItems) {
		int lowestId = GetLowestId(2);
		Item* it = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
		if (item.second == 1) {
			it->SetParameters(configParameters.child("entities").child("items").child("coin"), lowestId);
			it->SetItemType(ItemType::COIN);
		}
		else if (item.second == 2) {
			it->SetParameters(configParameters.child("entities").child("items").child("fireup"), lowestId);
			it->SetItemType(ItemType::FIREUP);
		}
		else {
			it->SetParameters(configParameters.child("entities").child("items").child("health"), lowestId);
			it->SetItemType(ItemType::HEALTH);
		}
		it->Start();
		it->SetPosition({ item.first.getX(), item.first.getY() });
		itemList.push_back(it);

		if (!contains) {
			pugi::xml_node newItem = saveFile.child("config").child("scene").child("items").append_child("item");
			newItem.append_attribute("id").set_value(lowestId);
			newItem.append_attribute("level").set_value(level);
			newItem.append_attribute("collected").set_value("false");
			newItem.append_attribute("x").set_value(item.first.getX());
			newItem.append_attribute("y").set_value(item.first.getY());

			saveFile.save_file("config.xml");
		}

	}

	levelsLoadedItems.push_back(level);


	//Poison
	RespawnPoison();
}

//Modify the XML and puts dead="true" to a enemy dead
void Scene::SaveKillEnemy(int id) {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("enemies").find_child_by_attribute("id", std::to_string(id).c_str());;

	enemiesNode.attribute("dead") = "true";
	saveFile.save_file("config.xml");
}

//Modify the XML and puts dead="false" to all enemies
void Scene::RestartEnemies() {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("enemies");

	for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {
		enemyNode.attribute("dead") = "false";
	}
	saveFile.save_file("config.xml");

}

void Scene::RemoveLevelEnemies(int levelRemove) {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("enemies");

	for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode;) {
		pugi::xml_node nextEnemy = enemyNode.next_sibling("enemy");
		if (enemyNode.attribute("level").as_int() == levelRemove)
			enemiesNode.remove_child(enemyNode);

		enemyNode = nextEnemy;
	}

	for (size_t i = 0; i < levelsLoadedEnemies.size(); i++) {
		if (levelsLoadedEnemies[i] == levelRemove)
			levelsLoadedEnemies.erase(levelsLoadedEnemies.begin() + i);
	}
	saveFile.save_file("config.xml");

}

int Scene::GetLowestId(int type) {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	int lowest = 1;
	if (type == 1) {
		pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("enemies");

		while (true) {
			bool idExists = false;

			for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {
				if (enemyNode.attribute("id").as_int() == lowest) {
					idExists = true;
					break;
				}
			}

			if (!idExists) break;

			lowest++;
		}
	}
	else {
		pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("items");

		while (true) {
			bool idExists = false;

			for (pugi::xml_node enemyNode = enemiesNode.child("item"); enemyNode; enemyNode = enemyNode.next_sibling("item")) {
				if (enemyNode.attribute("id").as_int() == lowest) {
					idExists = true;
					break;
				}
			}

			if (!idExists) break;

			lowest++;
		}
	}

	return lowest;
}

void Scene::SaveCollectedItem(int id) {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node itemsNode = saveFile.child("config").child("scene").child("entities").child("items");

	for (pugi::xml_node itemNode = itemsNode.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
		if (itemNode.attribute("id").as_int() == id) {
			itemNode.attribute("collected") = "true";
		}
	}
	saveFile.save_file("config.xml");
}

// Modify the XML and puts collected="false" to all items
void Scene::RestartItems() {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node itemsNode = saveFile.child("config").child("scene").child("entities").child("items");

	for (pugi::xml_node itemNode = itemsNode.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
		itemNode.attribute("collected") = "false";
	}
	saveFile.save_file("config.xml");
}


//Clear and create the Poisons of the level
void Scene::RespawnPoison() {
	std::list<Vector2D> list = Engine::GetInstance().map->GetPoisonList();

	for (int i = 0; i < poisonList.size();) {
		Engine::GetInstance().physics->DeleteBody(poisonList[i]->getBody());
		Engine::GetInstance().entityManager->DestroyEntity(poisonList[i]);
		poisonList.erase(poisonList.begin());
	}

	for (auto poison : list) {
		Poison* p = (Poison*)Engine::GetInstance().entityManager->CreateEntity(EntityType::POISON);
		p->SetParameters(configParameters.child("entities").child("poison"));
		p->Start();
		p->SetPosition({ poison.getX(), poison.getY() });
		poisonList.push_back(p);
	}
}

// -----------------------------
// Cleanup
// -----------------------------

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	SDL_DestroyTexture(img);
	enemyList.clear();
	itemList.clear();
	fireballList.clear();
	bonfireList.clear();
	poisonList.clear();

	return true;
}

