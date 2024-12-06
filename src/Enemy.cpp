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

	followPlayer = false;

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	levelEnemy = parameters.attribute("level").as_int();
	position.setX(parameters.attribute("x").as_int() * 8);
	position.setY(parameters.attribute("y").as_int() * 8);
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY() + texW, texW / 2, bodyType::DYNAMIC);

	sensor = Engine::GetInstance().physics.get()->CreateCircleSensor((int)position.getX(), (int)position.getY() + texW, texW * 4, bodyType::KINEMATIC);
	sensor->ctype = ColliderType::SENSOR;
	sensor->listener = this;

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();

	return true;
}

void Enemy::SetEnemyType(EnemyType et) {
	type = et;
}

bool Enemy::Update(float dt) {
	if (type == EnemyType::BAT) velocity = b2Vec2(0, 0);
	else velocity = b2Vec2(0, -GRAVITY_Y);

	if (followPlayer) {
		MovementEnemy(dt);
	}
	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX() + texW / 3, (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	b2Vec2 enemyPos = pbody->body->GetPosition();
	sensor->body->SetTransform({ enemyPos.x, enemyPos.y }, 0);

	if (currentAnimation == &die && currentAnimation->HasFinished()) {
		dead = true;
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
	if (posBread.getX() <= tilePos.getX()) {
		velocity.x = -0.1 * dt;
		flipType = SDL_FLIP_HORIZONTAL;
	}
	else {
		velocity.x = 0.1 * dt;
		flipType = SDL_FLIP_NONE;
	}

	if (type == EnemyType::BAT) {
		if (posBread.getY() <= tilePos.getY()) {
			velocity.y = -0.1 * dt;
		}
		else {
			velocity.y = 0.1 * dt;
		}
	}
}

bool Enemy::CleanUp()
{
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
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::DIE:
		dead = true;
		LOG("Collision DIE");
		break;
	case ColliderType::FIREBALL:
		if (physA->ctype != ColliderType::SENSOR) {
			currentAnimation = &die;
			LOG("Collision FIREBALL");
		}
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::SENSOR) {
			followPlayer = true;
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
	default:
		break;
	}
}

bool Enemy::IsDead() {
	return dead;
}