#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

enum class StateItem {
	IDLE, DIE
};

enum class ItemType {
	COIN, FIREUP, HEALTH, 
};


class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	
	bool HasCollision();

	bool Update(float dt);

	bool CleanUp();

public:

	bool dead = false;
	

private:

	SDL_Texture* texture;
	ItemType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, die;
	PhysBody* pbody;

	
	const char* texturePath;
	int levelItem;
	int texW, texH;
	float speed;
	int id;
	bool col;

	//L08 TODO 4: Add a physics to an item

};