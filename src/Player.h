#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Fireball.h"

struct SDL_Texture;

enum class Level {
	NEW, LOAD, DISABLED
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

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

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

	Level GetLevel() {
		return lvl;
	}

	void SetLevel(Level lv) {
		lvl = lv;
	}
public:

	//Declare player parameters
	float speed = 0.06f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//Audio fx
	int pickCoinFxId;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce = 0.37f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	bool isDying = false;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, run, jump, fall, die;
	DirectionPlayer dp;
	SDL_RendererFlip flipType;
	StatePlayer stPlayer;
	std::list<Fireball*> fireballs;
	Level lvl;
};