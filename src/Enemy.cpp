#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "tracy/Tracy.hpp"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

}

Enemy::~Enemy() {
	delete pathfinding;
}

bool Enemy::Awake() {
	return true;
}

bool Enemy::Start() {
	tempChangeAnimation = 120;
	followPlayer = false;
	speed = 1.9f;

	//Initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	levelEnemy = parameters.attribute("level").as_int();
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	crouch.LoadAnimations(parameters.child("animations").child("crouch"));
	attack.LoadAnimations(parameters.child("animations").child("attack"));
	dmg.LoadAnimations(parameters.child("animations").child("dmg"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	if (type == EnemyType::BAT) pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY() + texW, texW / 2, bodyType::DYNAMIC);
	else pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY() + texW, texW / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	//Sensor
	sensor = Engine::GetInstance().physics.get()->CreateCircleSensor((int)position.getX(), (int)position.getY() + texH, texW * 4, bodyType::KINEMATIC);
	sensor->ctype = ColliderType::SENSOR;
	sensor->listener = this;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);

	//Load Fx
	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");

	enemydSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("enemydSFX").attribute("path").as_string());
	bossSword = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("bossSwordSFX").attribute("path").as_string());
	bossDie = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("bossDieSFX").attribute("path").as_string());


	switch (type)
	{
	case EnemyType::EV_WIZARD:
		lifes = 3;
		directionLeft = false;
		break;
	case EnemyType::BAT:
		directionLeft = false;
		lifes = 4;
		break;
	case EnemyType::BOSS:
		directionLeft = true;
		lifes = 10;
		break;
	default:
		break;
	}

	if (type == EnemyType::BOSS) flipType = SDL_FLIP_HORIZONTAL;

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();

	return true;
}

void Enemy::SetEnemyType(EnemyType et) {
	type = et;
}

void Enemy::ShowEnemy(bool show) {

	if (!showing && show) {
		showing = show;
		//Add a physics to an item - initialize the physics body
		pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY() + texW, texW / 2, bodyType::DYNAMIC);

		sensor = Engine::GetInstance().physics.get()->CreateCircleSensor((int)position.getX(), (int)position.getY() + texH, texW * 4, bodyType::KINEMATIC);
		sensor->ctype = ColliderType::SENSOR;
		sensor->listener = this;

		//Assign collider type
		pbody->ctype = ColliderType::ENEMY;
		pbody->listener = this;


		// Set the gravity of the body
		if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);
	}
	else if (showing && !show) {
		showing = show;
		Engine::GetInstance().physics->DeleteBody(pbody->body);
		Engine::GetInstance().physics->DeleteBody(sensor->body);
	}
}

void Enemy::BossPattern(float dt) {
	velocity = b2Vec2(0, -GRAVITY_Y);

	if (currentAnimation == &crouch) texW = 48;
	else if (currentAnimation == &attack) texW = 80;
	else if (currentAnimation == &die) texW = 64;
	else texW = parameters.attribute("w").as_int();

	if ((currentAnimation == &attack || currentAnimation == &dmg) && currentAnimation->HasFinished()) currentAnimation = &idle;

	if (bossActive && !Engine::GetInstance().scene.get()->IsPause()) {

		if (bossCooldown >= 0)
			bossCooldown--;

		if (randomAttack == 2 && currentAnimation == &attack && bossCooldown == 100) {
			fireball = true;
			Engine::GetInstance().audio.get()->PlayFx(bossSword);
		}
		if (randomAttack == 3 && currentAnimation == &attack && (bossCooldown == 100 || bossCooldown == 80)) {
			fireball = true;
			Engine::GetInstance().audio.get()->PlayFx(bossSword);
		}


		if (bossCooldown <= 0) {

			if (currentAnimation == &walk) {
				currentAnimation = &idle;
				bossCooldown = 60;
			}
			else {
				randomAttack = rand() % 3 + 1;

				switch (randomAttack) {
				case 2:
					currentAnimation = &attack;

					currentAnimation->Reset();
					bossCooldown = 120;
					break;
				case 3:
					currentAnimation = &attack;

					currentAnimation->Reset();
					bossCooldown = 120;
					break;
				default:
					currentAnimation = &walk;
					bossCooldown = 160;
					de = directionLeft ? DirectionEnemy::LEFT : DirectionEnemy::RIGHT;
					break;
				}
			}
		}
		else if (bossCooldown <= 1 && currentAnimation == &walk) {
			directionLeft = !directionLeft;
			de = directionLeft ? DirectionEnemy::LEFT : DirectionEnemy::RIGHT;
		}

		if (currentAnimation == &walk) {
			if (directionLeft) {
				velocity.x = -speed * 2;
				flipType = SDL_FLIP_HORIZONTAL;
			}
			else {
				velocity.x = +speed * 2;
				flipType = SDL_FLIP_NONE;
			}
		}

	}

	if (isJumping)velocity = pbody->body->GetLinearVelocity();
	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	if (currentAnimation != &attack)
		Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX() + texW / 3, (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());
	else
		Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX() - 4, (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());

	currentAnimation->Update();

	b2Vec2 enemyPos = pbody->body->GetPosition();
	sensor->body->SetTransform({ enemyPos.x, enemyPos.y }, 0);

	if (currentAnimation == &die) {
		if (!audioDie) {
			Engine::GetInstance().audio.get()->PlayFx(bossDie);
			audioDie = true;
		}
		if (currentAnimation->HasFinished())dead = true;
	}

}

void Enemy::EnemyPattern(float dt) {
	if (!Engine::GetInstance().scene.get()->IsPause()) {
		if (type == EnemyType::BAT) velocity = b2Vec2(0, 0);
		else velocity = b2Vec2(0, -GRAVITY_Y);

		if (currentAnimation == &dmg && currentAnimation->HasFinished()) currentAnimation = &idle;

		if (coolDownPathFinding)
			coolDown--;

		if (coolDown <= 0) {
			coolDownPathFinding = false;
			coolDown = 100;
		}

		if (followPlayer && !coolDownPathFinding) {
			MovementEnemy(dt);
		}
		else {
			if (currentAnimation == &walk) {
				if (directionLeft) {
					velocity.x = -speed;
					flipType = SDL_FLIP_HORIZONTAL;
				}
				else {
					velocity.x = +speed;
					flipType = SDL_FLIP_NONE;
				}
			}

			if (type == EnemyType::EV_WIZARD) {
				tempChangeAnimation--;
				if (tempChangeAnimation <= 0) {
					if (currentAnimation == &walk) {
						currentAnimation = &idle;
						tempChangeAnimation = 120;
						de = DirectionEnemy::RIGHT;
					}
					else if (currentAnimation == &idle) {
						currentAnimation = &walk;
						tempChangeAnimation = 20;
						directionLeft = !directionLeft;
						de = DirectionEnemy::LEFT;
					}
				}
			}
		}

		pbody->body->SetLinearVelocity(velocity);

		b2Transform pbodyPos = pbody->body->GetTransform();
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
	}

	Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX(), (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (!Engine::GetInstance().scene.get()->IsPause()) {
		b2Vec2 enemyPos = pbody->body->GetPosition();
		sensor->body->SetTransform({ enemyPos.x, enemyPos.y }, 0);
	}
	else {
		pbody->body->SetLinearVelocity({ 0,0 });
	}

	if (currentAnimation == &die && currentAnimation->HasFinished()) {
		Engine::GetInstance().audio.get()->PlayFx(enemydSFX);
		dead = true;
	}
}

bool Enemy::Update(float dt) {
	ZoneScoped;

	if (showing) {
		if (lifes <= 0) currentAnimation = &die;
		if (type != EnemyType::BOSS) EnemyPattern(dt);
		else BossPattern(dt);
	}
	return true;
}

void Enemy::MovementEnemy(float dt) {
	//Reset
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);

	bool found = false;
	while (!found) {
		found = pathfinding->PropagateAStar(MANHATTAN);
		if (Engine::GetInstance().physics.get()->GetDebug())
			pathfinding->DrawPath();
	}

	int sizeBread = pathfinding->breadcrumbs.size();
	Vector2D posBread;
	if (sizeBread >= 2) posBread = pathfinding->breadcrumbs[pathfinding->breadcrumbs.size() - 2];
	else posBread = pathfinding->breadcrumbs[pathfinding->breadcrumbs.size() - 1];

	//Movement Enemy
	if (currentAnimation != &die) {
		if (posBread.getX() <= tilePos.getX()) {
			velocity.x = -speed;
			flipType = SDL_FLIP_HORIZONTAL;
		}
		else {
			velocity.x = speed;
			flipType = SDL_FLIP_NONE;
		}


		if (type == EnemyType::BAT) {
			if (posBread.getY() <= tilePos.getY()) {
				velocity.y = -speed;
			}
			else {
				velocity.y = speed;
			}
		}
	}
}

bool Enemy::CleanUp()
{
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

void Enemy::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}


void Enemy::ResetPath() {
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		isJumping = false;
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::DIE:
		dead = true;
		LOG("BOSS DIE");
		break;
	case ColliderType::FIREBALLPLAYER:
	case ColliderType::BIGFIREBALLPLAYER:
		if (physA->ctype != ColliderType::SENSOR) {
			if (physB->ctype == ColliderType::FIREBALLPLAYER) lifes--;
			else lifes -= 4;
			if (type != EnemyType::BOSS) {
				currentAnimation = &dmg;
				currentAnimation->Reset();
			}
			LOG("Collision FIREBALL");
		}
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::SENSOR) {
			followPlayer = true;
		}
		else {
			coolDownPathFinding = true;
		}
		break;
	default:
		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::SENSOR) {
			followPlayer = false;
		}
		break;
	case ColliderType::FIREBALLPLAYER:
		break;
	default:
		break;
	}
}

bool Enemy::IsDead() {
	return dead;
}