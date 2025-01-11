#pragma once

#include "Module.h"
#include "Player.h"
#include "Power.h"
#include "Enemy.h"
#include "Item.h"
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
enum class ITEM {
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

	void LoadAssets();

	void SetupUI();

	// Called before all Updates
	bool PreUpdate();

	void HandleCamera(Engine& engine);

	void HandlePowers();

	void HandleGui();

	void DebugMode();

	void ActiveBonfires();

	// Called each loop iteration
	bool Update(float dt);

	void SaveKillEnemy(int id);

	void SaveCollectedItem(int id);

	void RestartEnemies();

	void RemoveLevelEnemies(int level);

	int GetLowestId(int type);

	void RestartItems();

	void LoadState(LOAD load);

	void SaveState();

	void CreateEvents();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void RespawnPoison();

	int GetActualLevel() {
		return level;
	}

	bool IsPause() {
		return pause;
	}

	Vector2D GetPlayerPosition() {
		return player->GetPosition();
	}



	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	// --- SCENE PARAMETERS ---
	SDL_Texture* img;              // Scene background or general image
	int level;                     // Current level
	bool firstTimeLoad;            // Flag for the first-time loading of the scene
	bool exitGame;                 // Flag to exit the game

	// --- TILE AND DEBUG ---
	SDL_Texture* mouseTileTex = nullptr;  // Texture for the mouse tile
	std::string tilePosDebug = "[0,0]";   // Debug position of the tile
	bool once = false;                    // Flag for one-time operations

	// --- PLAYER AND ENTITIES ---
	Player* player;                       // Player instance
	std::vector<Enemy*> enemyList;        // List of enemies
	std::vector<Item*> itemList;          // List of items
	std::vector<Power*> fireballList;     // List of fireballs
	std::vector<Bonfire*> bonfireList;    // List of bonfires
	std::vector<Poison*> poisonList;      // List of poison traps
	std::vector<int> levelsLoadedBonfire; // Levels with bonfires loaded
	std::vector<int> levelsLoadedEnemies; // Levels with enemies loaded
	std::vector<int> levelsLoadedItems;   // Levels with items loaded
	int idEnemy = 1;                      // Identifier for enemies
	bool bossActive = false;              // Flag to indicate if the boss is active

	// --- BONFIRE ---
	int idBonfire;        // Bonfire ID
	int idNameBonfire;    // Bonfire name ID

	// --- AUDIO FX ---
	int bonfireSFX;       // Sound effect for bonfire activation
	int saveSFX;          // Sound effect for saving progress
	int loadSFX;          // Sound effect for loading progress

	// --- USER INTERFACE (UI) ---
	SDL_Texture* helpMenu;			 // Menu texture
	SDL_Texture* OptionsBook;	     // Menu texture
	SDL_Texture* TitleScreen;        //Title screen texture
	SDL_Texture* gui;				 // General GUI texture
	SDL_Texture* lifePlayer;         // Player life texture
	SDL_Texture* powerOn;
	SDL_Texture* pouch;
	SDL_Texture* pouchfull;
	SDL_Texture* powerOff;
	SDL_Texture* sliderBackground;   // Slider background texture
	SDL_Texture* sliderMovement;     // Slider movement texture
	SDL_Texture* menuButtonNormal;   // Slider menu
	SDL_Texture* menuButtonFocused;
	SDL_Texture* menuButtonPressed;
	
	bool showSettings = false;       // Flag to show settings menu
	bool showTp = false;
	          

	// --- FADE-IN EFFECT ---
	Uint8 alpha;         // Alpha transparency for fade-in effect
	bool fadeIn;         // Flag for fade-in state

	// --- HELP AND TELEPORTATION ---
	bool help;           // Flag for showing help
	bool enableTp;       // Teleportation optimization flag
	bool pause = false;	 // Flag for is pause the game or not
	int colRespawn;      // Column for respawn position
	int coordYMenuTp;    // Y-coordinate for menu teleportation

};