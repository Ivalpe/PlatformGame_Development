#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Timer.h"

struct SDL_Texture;

enum class StatePower {
	IDLE, DIE, FIRE
};

enum class DirectionPower {
	LEFT, RIGHT
};

enum class TypePower {
	FIREBALL, BIGFIREBALL, MELEEATTACKBOSS
};

class Power : public Entity
{
public:

	Power(EntityType et);
	virtual ~Power();

	bool Awake();

	bool Start(bool inv);

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters, TypePower tp) {
		this->parameters = parameters;
		power = tp;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	bool HasCollision();

	b2Body* getBody() {
		return pbody->body;
	}

public:

private:

	SDL_Texture* texture;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, explode, fire;
	DirectionPower df;
	SDL_RendererFlip flipType;
	StatePower statePower;
	PhysBody* pbody;
	TypePower power, bigPower;
	bool hasPlayedFire = false;


	Timer explosionTimer;
	float explosionTime;
	Timer fireTimer;
	float fireTime;

	bool col;
	bool inverted;

	int fireball1SFX;
	int fireball2SFX;
	int bigfireball1SFX;
	int bigfireball2SFX;

	int tempAttack = 1;
};
