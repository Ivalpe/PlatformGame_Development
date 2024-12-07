#pragma once
#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Bonfire.h"
#include "Poison.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	void DebugMode();

	// Called each loop iteration
	bool Update(float dt);

	void LoadState(bool initial);
	pugi::xml_node GetCurrentLevel();
	void SaveState();

	void CreateEvents();

	// Called before all Updates
	bool PostUpdate();

	void CreateEnemies();

	// Called before quitting
	bool CleanUp();

	Vector2D GetPlayerPosition();

	int GetActualLevel();

private:
	SDL_Texture* img;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	std::vector<Enemy*> enemyList;
	std::vector<Fireball*> fireballList;
	std::vector<Bonfire*> firecampList;
	std::vector<Poison*> poisonList;
	int level, colRespawn;
	bool help;
};