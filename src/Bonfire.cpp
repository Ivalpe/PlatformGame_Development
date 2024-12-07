#include "Bonfire.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"

Bonfire::Bonfire() : Entity(EntityType::BONFIRE) {
}

Bonfire::~Bonfire() {
}

bool Bonfire::Awake() {
	return true;
}

bool Bonfire::Start() {

	activeBonfire = false;
	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	on.LoadAnimations(parameters.child("animations").child("on"));
	sf = StateBonfire::IDLE;
	currentAnimation = &idle;

	return true;
}

void Bonfire::ActiveBonfire() {
	sf = StateBonfire::ON;
	currentAnimation = &on;
	activeBonfire = true;
}

void Bonfire::DisableBonfire() {
	activeBonfire = false;
}

bool Bonfire::IsActive() {
	return activeBonfire;
}

bool Bonfire::Update(float dt)
{

	Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Bonfire::CleanUp()
{
	return true;
}

void Bonfire::SetPosition(Vector2D pos) {
	position.setX(pos.getX() + texW / 2);
	position.setY(pos.getY() + texH / 2);
}

Vector2D Bonfire::GetPosition() {
	return position;
}