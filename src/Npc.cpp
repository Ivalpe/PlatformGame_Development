#include "Npc.h"
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

Npc::Npc() : Entity(EntityType::NPC)
{

}

Npc::~Npc() {
}

bool Npc::Awake() {
	return true;
}

bool Npc::Start() {
	speed = 1.9f;


	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();
	flip = parameters.attribute("flip").as_bool() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY() + texW, texW / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::NPC;
	pbody->listener = this;

	return true;
}

bool Npc::Update(float dt) {
	ZoneScoped;

	velocity = b2Vec2(0, -GRAVITY_Y);

	if (isJumping)
		velocity = pbody->body->GetLinearVelocity();

	pbody->body->SetLinearVelocity(velocity);

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	if (!isJumping) {
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	Engine::GetInstance().render.get()->DrawTexture(texture, flip, (int)position.getX() + texW / 3, (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

void Npc::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

bool Npc::CleanUp()
{
	return true;
}

void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		isJumping = false;
		LOG("Collision PLATFORM");
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::DIE:
		LOG("Collision DIE");
		break;
	case ColliderType::FIREBALLPLAYER:
	case ColliderType::BIGFIREBALLPLAYER:
		break;
	case ColliderType::ATTACKBOSS:
		LOG("Collision FIREBALLENEMY");
		break;
	case ColliderType::PLAYER:
		break;
	default:
		break;
	}
}

void Npc::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::FIREBALLPLAYER:
		break;
	default:
		break;
	}
}