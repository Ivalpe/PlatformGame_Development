#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Log.h"

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

	void SetParameters(pugi::xml_node parameters, int idBonfire) {
		this->parameters = parameters;
		id = idBonfire;
	}

	StateBonfire GetState() {
		return sf;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	b2Body* getBody() {
		return pbody->body;
	}

	int getId() {
		return id;
	}

	void ShowBonfire(bool show) {
		showBonfire = show;
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
	int id;
	bool showBonfire = false;
	
	
};
#pragma once
