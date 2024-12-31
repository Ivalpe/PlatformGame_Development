#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Bonfire.h"
#include "Poison.h"
#include "GuiControlButton.h"
#include "UI.h"

struct SDL_Texture;

enum class LOAD {
	INITIAL, RESPAWN, DEBUG
};

enum class ENEMY {
	CREATEALL, CLEARDEADS, CREATEXML
};

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

	void ActiveBonfires();

	// Called each loop iteration
	bool Update(float dt);

	void SaveKillEnemy(int id);

	void RestartEnemies();

	void LoadState(LOAD load);
	pugi::xml_node SearchLevel(int lvl);
	void SaveState();

	void CreateEvents();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void ClearEnemyList();
	void RespawnPoison();

	int GetActualLevel() {
		return level;
	}

	Vector2D GetPlayerPosition() {
		return player->GetPosition();
	}


	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = false;
	SDL_Texture* img;
	UI ui;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	std::vector<Enemy*> enemyList;
	std::vector<Fireball*> fireballList;
	std::vector<Bonfire*> bonfireList;
	std::vector<Poison*> poisonList;
	std::vector<int> bonfireCharged;
	int level, colRespawn, coordYMenuTp;
	bool help, firstTimeBonfires, exitGame; //enableTp is for optimization and not making for every frame
	ENEMY enState;

	int bonfireSFX, saveSFX, loadSFX, idBonfire, idNameBonfire;
};