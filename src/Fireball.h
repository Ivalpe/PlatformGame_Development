#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Timer.h"

struct SDL_Texture;

enum class StateFireball {
	IDLE,DIE
};

enum class DirectionFireball {
	LEFT, RIGHT
};

class Fireball : public Entity
{
public:

	Fireball();
	virtual ~Fireball();

	bool Awake();

	bool Start(bool inv);

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
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
	Animation idle, explode;
	DirectionFireball df;
	SDL_RendererFlip flipType;
	StateFireball stFireball;
	PhysBody* pbody;

	Timer explosionTimer;
	float explosionTime;
	Timer fireTimer;
	float fireTime;

	bool col;
	bool inverted;

	int fireball1SFX;
	int fireball2SFX;
};
#pragma once
