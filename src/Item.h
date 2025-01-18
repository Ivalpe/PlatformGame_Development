#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Module.h"

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

	void SetItemType(ItemType it);

	ItemType GetItemType() {
		return ittype;
	}

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters, int id) {
		this->parameters = parameters;
		this->id = id;
	}

	int GetId() {
		return id;
	}

	bool HasCollision() {
		return col;
	}

	bool IsCollected() {
		return collected;
	}
	
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	b2Body* getBody() {
		return pbody->body;
	}

	void ShowItem(bool show);

	bool IsShowing() {
		return showing;
	}

	void Respawn() {
		collected = false;
		stItem = StateItem::IDLE;
	}

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	
	
	ItemType ittype;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, collect;
	StateItem stItem;
	b2Vec2 velocity;

	PhysBody* pbody;
	int levelItem;
	bool collected;
	int item_pickupSFX;   
	
	float speed;
	int id;
	bool col;
	bool showing = true;
};