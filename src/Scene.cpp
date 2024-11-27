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
	//Make the camera movement independent of framerate
	float camSpeed = 1;

	/*
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y -= ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y += ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x -= ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x += ceil(camSpeed * dt);
		*/
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
		fireball->Start();
		fireball->SetPosition(player->GetPosition());
		fireballList.push_back(fireball);
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

	//Load Level 1 when you are in level 0 and press number 1
	if (level == 0 && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		level = 1;
		for (pugi::xml_node mapNode = configParameters.child("levels").child("map"); mapNode; mapNode = mapNode.next_sibling("map"))
		{
			if (mapNode.attribute("number").as_int() == level) {
				Engine::GetInstance().map->Load("Assets/Maps/", mapNode.attribute("name").as_string());
				break;
			}
		}
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