#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

enum class StateItem {
	IDLE
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

	void SetItemType(ItemType it);

	
	bool HasCollision();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	int GetId() {
		return id;
	}

	
	
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	bool IsCollected();

	b2Body* getBody() {
		return pbody->body;
	}

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	
	
	ItemType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, die;
	StateItem stItem;
	b2Vec2 velocity;

	PhysBody* pbody;
	int levelItem;
	bool collected;
	
	float speed;
	int id;
	bool col;

	//L08 TODO 4: Add a physics to an item

};