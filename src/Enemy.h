#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

#define ENEMY_SPEED			1

enum class StateEnemy {
	IDLE, WALK, DIE
};

enum class DirectionEnemy {
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

	void SetEnemyType(EnemyType et);

	bool Update(float dt);

	void MovementEnemy(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	int GetId() {
		return id;
	}

	void SetPosition(Vector2D pos);
	Vector2D GetPosition();
	void ResetPath();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	bool IsDead();

public:

	b2Body* getBody() {
		return pbody->body;
	}

	b2Body* getSensorBody() {
		return sensor->body;
	}


private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	bool isDying = false;
	
	EnemyType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, walk, die;
	DirectionEnemy de;
	SDL_RendererFlip flipType;
	StateEnemy stEnemy;

	PhysBody* pbody;
	PhysBody* sensor;
	Pathfinding* pathfinding;
	int levelEnemy;
	bool dead, followPlayer;
	b2Vec2 velocity;
	float speed;
	int id;

	int enemydSFX;
	int tempChangeAnimation;
	bool directionLeft;
};
