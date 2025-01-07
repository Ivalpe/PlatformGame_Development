#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Power.h"

struct SDL_Texture;

enum class Level {
	NEXTLVL, WIN, DISABLED
};

enum class StatePlayer {
	IDLE, RUN, JUMP, FALL, DIE
};

enum class DirectionPlayer {
	LEFT, RIGHT
};

class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L08 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void ChangeDebug();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void Respawn();

	void SetPosition(Vector2D posPlayer);

	Vector2D GetPosition() {
		return position;
	}

	StatePlayer GetState() {
		return stPlayer;
	}

	int GetX() {
		return position.getX();
	}

	int GetY() {
		return position.getY();
	}

	DirectionPlayer GetDirection() {
		return dp;
	}

	void ActivePlayer() {
		disablePlayer = false;
	}

	void DisablePlayer() {
		disablePlayer = true;
	}

	Level GetLevel() {
		return lvl;
	}

	void SetLevel(Level lv) {
		lvl = lv;
	}

	int GetLifes() {
		return lifes;
	}
public:


	// --- PLAYER PARAMETERS ---
	float speed = 3.0f;          // Movement speed
	SDL_Texture* texture = NULL; // Player texture
	int texW, texH;              // Texture dimensions
	bool disablePlayer = false;  // Flag to disable player controls

	// --- PLAYER STATE ---
	bool isJumping = false;      // Flag to check if the player is jumping
	bool isDying = false;        // Flag to check if the player is dying
	bool isDmg = false;          // Flag to check if the player is damaged
	bool hasLanded = false;      // Flag to check if the player has landed
	int lifes;                   // Number of lives
	int coins;                   // Number of coins collected
	bool firePower = false;              // Flag for fire power-up
	bool debugMode;              // Flag for debug mode

	// --- PHYSICS ---
	PhysBody* pbody;             // Player physics body
	float jumpForce = 0.37f;     // Jump force

	// --- AUDIO FX ---
	int pickCoinFxId;            // Audio ID for coin pickup
	int pdeathSFX;               // Audio ID for player death
	int acidkillSFX;             // Audio ID for acid death
	int jumpSFX;                 // Audio ID for jumping
	int landSFX;                 // Audio ID for landing
	int damageSFX;               // Audio ID for taking damage

	// --- AUDIO TIMERS ---
	float deathSoundTimer = 0.0f;
	const float deathSoundCooldown = 1.0f;
	float damageSoundTimer = 0.0f;
	const float damageSoundCooldown = 1.0f;
	float landSoundTimer = 0.0f;
	const float landSoundCooldown = 1.0f;

	// --- ANIMATION ---
	Animation* currentAnimation = nullptr; // Current active animation
	Animation idle;                        // Idle animation
	Animation run;                         // Run animation
	Animation jump;                        // Jump animation
	Animation fall;                        // Fall animation
	Animation die;                         // Death animation
	Animation dmg;                         // Damage animation

	// --- PLAYER DIRECTION ---
	DirectionPlayer dp;           // Player direction
	SDL_RendererFlip flipType;    // SDL Renderer flip type

	// --- PLAYER STATE MACHINE ---
	StatePlayer stPlayer;         // Player state

	// --- LEVEL CONFIGURATION ---
	Level lvl;                    // Current player level

	// --- XML PARAMETERS ---
	pugi::xml_node parameters;    // XML parameters

};