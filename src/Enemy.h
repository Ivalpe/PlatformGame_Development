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
	EV_WIZARD, BAT, BOSS,
};


class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	void SetEnemyType(EnemyType et);

	EnemyType GetType() {
		return type;
	}

	bool Update(float dt);

	void MovementEnemy(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters, int id) {
		this->parameters = parameters;
		this->id = id;
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

	void Respawn() {
		dead = false;
		switch (type)
		{
		case EnemyType::EV_WIZARD:
			lifes = 3;
			break;
		case EnemyType::BAT:
			lifes = 4;
			break;
		case EnemyType::BOSS:
			lifes = 10;
			break;
		default:
			break;
		}
	}

	void EnemyPattern(float dt);
	void BossPattern(float dt);

	b2Body* getBody() {
		return pbody->body;
	}

	b2Body* getSensorBody() {
		return sensor->body;
	}

	void ActiveBoss() {
		bossActive = true;
	}

	bool GetBossFireball() {
		return fireball;
	}

	void SetBossFireball(bool f) {
		fireball = f;
	}

	DirectionEnemy GetDirection() {
		return de;
	}

	void ShowEnemy(bool show);

	bool IsShowing() {
		return showing;
	}

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	bool isDying = false;

	EnemyType type;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle, walk, fly, die, crouch, attack, dmg;
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

	int enemydSFX, bossSword, bossDie, bossDmg;
	int tempChangeAnimation;
	bool directionLeft;
	bool coolDownPathFinding = false;
	int coolDown = 100;

	//BOSS
	bool bossActive, fireball = false;
	int bossCooldown = 120;
	int lifes;
	bool isJumping = false;
	bool showing = true;
	int randomAttack = 0;
	bool audioDie = false;
};
