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
#include "fireball.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"
#include "GuiControl.h"
#include "GuiManager.h"


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
	LOG("Loading Scene");
	bool ret = true;

	help = false;
	colRespawn = 120;
	level = 0;

	//Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));

	//Create a new item using the entity manager and set the position to (200, 672) to test
	Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	item->position = Vector2D(200, 672);

	coordYMenuTp = 350;

	enableTp = false;
	for (auto button : buttonList) {
		button->Disable();
	}
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Texture to highligh mouse position 
	mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");

	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");

	bonfireSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("bonfireSFX").attribute("path").as_string());
	loadSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("loadsSFX").attribute("path").as_string());
	saveSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("saveSFX").attribute("path").as_string());

	Engine::GetInstance().audio->PlayMusic(configParameters.child("audio").child("music").child("Music1SFX").attribute("path").as_string(), 0.0f);
	//Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", configParameters.child("levels").child("map").attribute("name").as_string());
	RestartEnemies();
	enState = ENEMY::CREATEALL;
	CreateEvents();

	return true;
}

// -----------------------------
// Game Loop Functions
// -----------------------------

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	//Debug Mode
	DebugMode();

	//Camera
	if (level != 0) {
		Engine::GetInstance().render.get()->camera.x = ((player->GetX() * -1) + 200) * 2;
		int cameraX = Engine::GetInstance().render.get()->camera.x;
		int cameraMaxX = Engine::GetInstance().map.get()->GetWidth() * 8 * -1 - (240 * 8);
		if (cameraX >= 0) Engine::GetInstance().render.get()->camera.x = 0;
		if (cameraX <= cameraMaxX) Engine::GetInstance().render.get()->camera.x = cameraMaxX;
	}

	//Open Help
	if (help) Engine::GetInstance().render.get()->DrawTexture(Engine::GetInstance().textures.get()->Load("Assets/Textures/HelpMenu.png"), SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2, 0);

	// Shoot
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

		Fireball* fireball = (Fireball*)Engine::GetInstance().entityManager->CreateEntity(EntityType::FIREBALL);
		fireball->SetParameters(configParameters.child("entities").child("fireball"));
		if (player->GetDirection() == DirectionPlayer::LEFT) fireball->Start(true);
		else fireball->Start(false);

		Vector2D playerPos = player->GetPosition();
		if (player->GetDirection() == DirectionPlayer::LEFT) fireball->SetPosition({ playerPos.getX() - 4, playerPos.getY() + 14 });
		else fireball->SetPosition({ playerPos.getX() + 32, playerPos.getY() + 14 });

		fireballList.push_back(fireball);
	}

	// Destroy fireballs collided
	for (int i = 0; i < fireballList.size(); i++) {
		if (fireballList[i]->HasCollision()) {
			Engine::GetInstance().physics->DeleteBody(fireballList[i]->getBody());
			Engine::GetInstance().entityManager->DestroyEntity(fireballList[i]);
			fireballList.erase(fireballList.begin() + i);
			i--;
		}
	}

	// Destroy died enemies
	enState = ENEMY::CLEARDEADS;
	ClearEnemyList();

	bool tp = false;
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	// Active bonfire if player touch it
	for (auto bonfire : bonfireList) {
		if (player->GetPosition().getX() >= bonfire->GetPosition().getX() - 16 && player->GetPosition().getX() <= bonfire->GetPosition().getX() + 8 &&
			player->GetPosition().getY() >= bonfire->GetPosition().getY() - 16 && player->GetPosition().getY() <= bonfire->GetPosition().getY() + 8) {

			int posXBonfire = bonfire->GetPosition().getX();
			pugi::xml_node bonfires = saveFile.child("config").child("scene").child("bonfires").find_child_by_attribute("x", std::to_string(posXBonfire).c_str());
			tp = true;
			if (bonfires.attribute("activated").as_bool() == false) {
				buttonList.push_back((GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, buttonList.size() + 1, "Hoguera", { 520, coordYMenuTp += 40, 120,20 }, this));
				bonfire->ActiveBonfire();
				Engine::GetInstance().audio.get()->PlayFx(bonfireSFX);

				saveFile.child("config").child("scene").child("entities").child("player").attribute("x").set_value(bonfire->GetPosition().getX());
				saveFile.child("config").child("scene").child("entities").child("player").attribute("y").set_value(bonfire->GetPosition().getY());
				bonfires.attribute("activated").set_value("true");
				saveFile.save_file("config.xml");
			}
		}
	}

	if (tp) {
		enableTp = true;
		for (auto button : buttonList) {
			button->Enable();
		}
	}
	else if (enableTp) {
		enableTp = false;
		for (auto button : buttonList) {
			button->Disable();
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

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//New Level
	if (level == 0 && player->GetLevel() == Level::NEW) {
		level++;
		for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map"))
		{
			if (mapNode.attribute("number").as_int() == level) {
				Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				enState = ENEMY::CREATEALL;
				CreateEvents();

				pugi::xml_node currentLevel = SearchLevel(level);
				Vector2D posPlayer;
				posPlayer.setX(currentLevel.attribute("ix").as_int());
				posPlayer.setY(currentLevel.attribute("iy").as_int() - 16);

				player->SetPosition(posPlayer);

				break;
			}
		}
		player->SetLevel(Level::DISABLED);
	}

	//Load Level
	else if (level == 0 && player->GetLevel() == Level::LOAD) {
		level++;
		for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map"))
		{
			if (mapNode.attribute("number").as_int() == level) {
				Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				LoadState(LOAD::RESPAWN);
				enState = ENEMY::CREATEXML;
				CreateEvents();
				break;
			}
		}
		player->SetLevel(Level::DISABLED);
	}

	return ret;
}

// -----------------------------
// Debug
// -----------------------------

void Scene::DebugMode() {

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN ||
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		int previousLevel = level;
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) level = 0;
		else level = 1;
		for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map")) {
			if (mapNode.attribute("number").as_int() == level) {
				if (level != previousLevel) Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				LoadState(LOAD::INITIAL);
				CreateEvents();
				break;
			}
		}
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

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

// -----------------------------
// State Management
// -----------------------------

void Scene::LoadState(LOAD load) {
	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	Engine::GetInstance().render.get()->camera.x = 0;

	if (result == NULL) {
		LOG("Error");
	}

	pugi::xml_node currentLevel = SearchLevel(level);
	Vector2D posPlayer;
	if (load == LOAD::INITIAL) {
		posPlayer.setX(currentLevel.attribute("ix").as_int());
		posPlayer.setY(currentLevel.attribute("iy").as_int() - 16);
	}
	else if (load == LOAD::RESPAWN) {
		posPlayer.setX(loadFile.child("config").child("scene").child("entities").child("player").attribute("x").as_int());
		posPlayer.setY(loadFile.child("config").child("scene").child("entities").child("player").attribute("y").as_int() - 16);
	}
	else {
		int lvlSave = loadFile.child("config").child("scene").child("entities").child("player").attribute("dlevel").as_int();
		if (lvlSave != level) {
			Engine::GetInstance().map->Load("Assets/Maps/", SearchLevel(lvlSave).attribute("name").as_string());
			level = lvlSave;
		}
		posPlayer.setX(loadFile.child("config").child("scene").child("entities").child("player").attribute("dx").as_int());
		posPlayer.setY(loadFile.child("config").child("scene").child("entities").child("player").attribute("dy").as_int() - 16);

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

pugi::xml_node Scene::SearchLevel(int lvl) {
	for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map")) {
		if (mapNode.attribute("number").as_int() == lvl) {
			return mapNode;
		}
	}
}

// -----------------------------
// Entity Creation and Management
// -----------------------------

void Scene::CreateEvents() {
	std::list<Vector2D> list;

	//Firecamps
	for (int i = 0; i < bonfireList.size();) {
		Engine::GetInstance().entityManager->DestroyEntity(bonfireList[i]);
		bonfireList.erase(bonfireList.begin());
	}
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	saveFile.child("config").child("scene").child("bonfires").remove_children();
	saveFile.save_file("config.xml");
	list = Engine::GetInstance().map->GetBonfireList();
	for (auto bonfire : list) {
		Bonfire* fc = (Bonfire*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BONFIRE);
		fc->SetParameters(configParameters.child("entities").child("firecamp"));
		fc->SetPosition({ bonfire.getX(), bonfire.getY() });
		fc->Start();
		bonfireList.push_back(fc);

		pugi::xml_node new_bonfire = saveFile.child("config").child("scene").child("bonfires").append_child("bonfire");
		new_bonfire.append_attribute("level").set_value(level);
		new_bonfire.append_attribute("activated").set_value("false");
		new_bonfire.append_attribute("x").set_value(bonfire.getX());
		saveFile.save_file("config.xml");
	}

	//This works but delete later AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	/*for (int i = 0; i < buttonList.size();) {
		buttonList.erase(buttonList.begin());
	}*/

	//Poison
	RespawnPoison();

	//Enemies
	ClearEnemyList();
}

//Modify the XML and puts dead="true" to a enemy dead
void Scene::SaveKillEnemy(int id) {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("entities").child("enemies");

	for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {
		if (enemyNode.attribute("id").as_int() == id) {
			enemyNode.attribute("dead") = "true";
		}
	}
	saveFile.save_file("config.xml");
}

//Modify the XML and puts dead="false" to all enemies
void Scene::RestartEnemies() {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");
	pugi::xml_node enemiesNode = saveFile.child("config").child("scene").child("entities").child("enemies");

	for (pugi::xml_node enemyNode = enemiesNode.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy")) {
		enemyNode.attribute("dead") = "false";
	}
	saveFile.save_file("config.xml");

}

//Clear all Enemy List or remove the dead enemies
void Scene::ClearEnemyList() {

	switch (enState)
	{
	case ENEMY::CREATEALL:
		for (int i = 0; i < enemyList.size(); i++) {
			Engine::GetInstance().physics->DeleteBody(enemyList[i]->getBody());
			Engine::GetInstance().physics->DeleteBody(enemyList[i]->getSensorBody());
			Engine::GetInstance().entityManager->DestroyEntity(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
			i--;
		}

		for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			if (level == enemyNode.attribute("level").as_int()) {
				Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);

				std::string enemyType = enemyNode.attribute("name").as_string();
				if (enemyType == "evilwizard") enemy->SetEnemyType(EnemyType::EV_WIZARD);
				else if (enemyType == "bat") enemy->SetEnemyType(EnemyType::BAT);

				enemy->SetParameters(enemyNode);
				enemy->Start();
				enemyList.push_back(enemy);
			}
		}
		break;
	case ENEMY::CLEARDEADS:
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i]->IsDead()) {
				SaveKillEnemy(enemyList[i]->GetId());
				Engine::GetInstance().physics->DeleteBody(enemyList[i]->getBody());
				Engine::GetInstance().physics->DeleteBody(enemyList[i]->getSensorBody());
				Engine::GetInstance().entityManager->DestroyEntity(enemyList[i]);
				enemyList.erase(enemyList.begin() + i);
				i--;
			}
		}
		break;
	case ENEMY::CREATEXML:
		for (int i = 0; i < enemyList.size(); i++) {
			Engine::GetInstance().physics->DeleteBody(enemyList[i]->getBody());
			Engine::GetInstance().physics->DeleteBody(enemyList[i]->getSensorBody());
			Engine::GetInstance().entityManager->DestroyEntity(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
			i--;
		}

		for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			if (level == enemyNode.attribute("level").as_int() && !enemyNode.attribute("dead").as_bool()) {
				Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);

				std::string enemyType = enemyNode.attribute("name").as_string();
				if (enemyType == "evilwizard") enemy->SetEnemyType(EnemyType::EV_WIZARD);
				else if (enemyType == "bat") enemy->SetEnemyType(EnemyType::BAT);

				enemy->SetParameters(enemyNode);
				enemy->Start();
				enemyList.push_back(enemy);
			}
		}
		break;
	default:
		break;
	}
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
	fireballList.clear();
	bonfireList.clear();
	poisonList.clear();

	return true;
}

