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
#include "Map.h"
#include "Item.h"
#include "Enemy.h"

Scene::Scene() : Module()
{
	name = "scene";
	img = nullptr;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	level = 0;

	//Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));

	//Create a new item using the entity manager and set the position to (200, 672) to test
	Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	item->position = Vector2D(200, 672);

	for (pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
	{
		Enemy* enemy = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
		enemy->SetParameters(enemyNode);
		enemyList.push_back(enemy);
	}
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Call the function to load the map. 
	Engine::GetInstance().map->Load("Assets/Maps/", configParameters.child("levels").child("map").attribute("name").as_string());
	for (auto firecamp : Engine::GetInstance().map->posFirecamp) {
		LOG("POS: %f %f", firecamp->getX(), firecamp->getY());
		Firecamp* fc = (Firecamp*)Engine::GetInstance().entityManager->CreateEntity(EntityType::FIRECAMP);
		fc->SetParameters(configParameters.child("entities").child("firecamp"));
		fc->SetPosition({ firecamp->getX(), firecamp->getY() });
		fc->Start();
		firecampList.push_back(fc);
	}
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (level != 0) {
		Engine::GetInstance().render.get()->camera.x = ((player->GetX() * -1) + 200) * 2;

		if (Engine::GetInstance().render.get()->camera.x >= 0)
			Engine::GetInstance().render.get()->camera.x = 0;

		if (Engine::GetInstance().render.get()->camera.x <= -448)
			Engine::GetInstance().render.get()->camera.x = -448;

	}

	// Shoot
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		Fireball* fireball = (Fireball*)Engine::GetInstance().entityManager->CreateEntity(EntityType::FIREBALL);
		fireball->SetParameters(configParameters.child("entities").child("fireball"));
		if (player->GetDirection() == DirectionPlayer::LEFT) fireball->Start(true);
		else fireball->Start(false);

		Vector2D playerPos = player->GetPosition();
		if (player->GetDirection() == DirectionPlayer::LEFT)
			fireball->SetPosition({ playerPos.getX() - 8, playerPos.getY() + 14 });
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
	for (int i = 0; i < enemyList.size(); i++) {
		if (enemyList[i]->IsDead()) {
			Engine::GetInstance().physics->DeleteBody(enemyList[i]->getBody());
			Engine::GetInstance().entityManager->DestroyEntity(enemyList[i]);
			enemyList.erase(enemyList.begin() + i);
			i--;
		}
	}

	//Engine::GetInstance().physics.get()->CreateRectangle(firecampList[0]->GetPosition().getX() + 8, firecampList[0]->GetPosition().getY() + 8, 16, 16, bodyType::STATIC);

	//LOG("PLAYER: %f, CAMP: %f", player->GetPosition().getX(), firecampList[0]->GetPosition().getX() - 8);
	//LOG("PLAYER: %f, CAMP: %f", player->GetPosition().getX(), firecampList[0]->GetPosition().getX() + 8);

	for (auto firecamp : firecampList) {
		if (firecamp->GetState() == StateFirecamp::IDLE &&
			player->GetPosition().getX() >= firecamp->GetPosition().getX() - 16 && player->GetPosition().getX() <= firecamp->GetPosition().getX() + 8 &&
			player->GetPosition().getY() >= firecamp->GetPosition().getY() - 16 && player->GetPosition().getY() <= firecamp->GetPosition().getY() + 8) {
			firecamp->ActiveFirecamp();
		}
	}

	return true;
}

void Scene::LoadState() {
	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL) {
		LOG("Error");
	}

	Vector2D posPlayer;
	posPlayer.setX(loadFile.child("config").child("scene").child("entities").child("player").attribute("ix").as_int());
	posPlayer.setY(loadFile.child("config").child("scene").child("entities").child("player").attribute("iy").as_int());

	player->SetPosition(posPlayer);
}

void Scene::SaveState() {
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	if (result == NULL) {
		LOG("Error");
	}

	Vector2D playerPos = player->GetPosition();
	saveFile.child("config").child("scene").child("entities").child("player").attribute("x").set_value(playerPos.getX());
	saveFile.child("config").child("scene").child("entities").child("player").attribute("y").set_value(playerPos.getY());

	saveFile.save_file("config.xml");
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		SaveState();
		ret = false;
	}

	if (level == 0 && player->GetLevel() == Level::NEW) {
		level++;
		for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map"))
		{
			if (mapNode.attribute("number").as_int() == level) {
				Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				break;
			}
		}
		player->SetLevel(Level::DISABLED);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	SDL_DestroyTexture(img);
	enemyList.clear();
	fireballList.clear();

	return true;
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

int Scene::GetActualLevel() {
	return level;
}