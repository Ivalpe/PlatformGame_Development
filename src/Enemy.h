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

	void CreateHitbox();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);
	Vector2D GetPosition();
	void ResetPath();
	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool IsDead();

public:
	float speed = 0;
	void MoveY();
	void MoveX();
	EnemyType getType() const;
	void Damaged(int dmg);
	int getLife() const;

	b2Body* getBody() {
		return pbody->body;
	}

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	bool isDying = false;
	
	EnemyType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, walk;
	DirectionEnemy de;
	SDL_RendererFlip flipType;
	StateEnemy stEnemy;

	PhysBody* pbody;
	Pathfinding* pathfinding;
	bool dead;
	
};
