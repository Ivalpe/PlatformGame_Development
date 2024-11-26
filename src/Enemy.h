#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

#define ENEMY_SPEED			1

enum class StatePlayer {
	IDLE, WALKING, DIE
};

enum class DirectionPlayer {
	LEFT, RIGHT
};

enum class EnemyType { 
	EV_WIZARD, BAT, KILLERBUNNY, 
};


class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void ResetPath();

public:

	void MoveY();
	void MoveX();
	EnemyType getType() const;
	void Damaged(int dmg);
	int getLife() const;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	
	EnemyType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, walk;

	PhysBody* pbody;
	Pathfinding* pathfinding;

	float moveTimer = 0.0f;       
	float idleDuration = 2.0f;   
	float moveDuration = 3.0f;   
	bool isIdle = false;         
	int moveDirection = 1;
	
};
