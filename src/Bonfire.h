#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

enum class StateBonfire {
	IDLE, ON
};

class Bonfire : public Entity
{
public:

	Bonfire();
	virtual ~Bonfire();

	bool Awake();

	bool Start();

	void ActiveBonfire();

	void DisableBonfire();

	bool IsShowing();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	StateBonfire GetState() {
		return sf;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	b2Body* getBody() {
		return pbody->body;
	}

public:

private:

	SDL_Texture* texture;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, on;
	PhysBody* pbody;
	StateBonfire sf;
	bool activeBonfire;

	
	
};
#pragma once
