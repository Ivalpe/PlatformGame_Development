#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Fireball.h"

struct SDL_Texture;

enum class Level {
	NEW, LOAD, NEXTLVL, WIN, DISABLED
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
public:

	//Declare player parameters
	float speed = 3.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//Audio fx
	int pickCoinFxId;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce = 0.37f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	bool isDying = false;
	bool isDmg = false;
	bool hasLanded = false;
	bool disablePlayer = false;

	float deathSoundTimer = 0.0f;
	const float deathSoundCooldown = 1.0f;
	float damageSoundTimer = 0.0f;
	const float damageSoundCooldown = 1.0f;
	float landSoundTimer = 0.0f;
	const float landSoundCooldown = 1.0f;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, run, jump, fall, die, dmg;
	DirectionPlayer dp;
	SDL_RendererFlip flipType;
	StatePlayer stPlayer;
	std::list<Fireball*> fireballs;
	Level lvl;
	int lifes;
	bool debugMode;

	int pdeathSFX;
	int acidkillSFX;
	int jumpSFX;
	int landSFX;
	int damageSFX;
};