#include "Poison.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Player.h"
#include "Physics.h"
#include "Map.h"
#include <random>

Poison::Poison() : Entity(EntityType::POISON) {
}

Poison::~Poison() {
}

bool Poison::Awake() {
	return true;
}

bool Poison::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations

	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Load Fx
	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");

	acidkillSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("audio").child("fx").child("acidkillSFX").attribute("path").as_string());

	//Random number between 0 and 12
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 12);

	//Random Frame
	currentAnimation->SetFrame(dist(gen));

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX(), (int)position.getY(), 16, 32, bodyType::STATIC);
	position.setX(METERS_TO_PIXELS((int)position.getX()));
	position.setY(METERS_TO_PIXELS((int)position.getY()));

	//Assign collider type
	pbody->ctype = ColliderType::DIE;
	pbody->listener = this;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	return true;
}

bool Poison::Update(float dt)
{
	


	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH);

	Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX() + 8, (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Poison::CleanUp()
{
	return true;
}

void Poison::SetPosition(Vector2D pos) {
	pos.setX(pos.getX());
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Poison::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Poison::OnCollision(PhysBody* physA, PhysBody* physB) {
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
	case ColliderType::PLAYER:

		if (acidDeathSoundTimer <= 0.0f) {
			Engine::GetInstance().audio.get()->PlayFx(acidkillSFX);
			acidDeathSoundTimer = acidDeathSoundCooldown; // Reiniciar el temporizador para evitar que se repita
		}
		LOG("Collision DIE");
		break;
	case ColliderType::DIE:
		
			
		LOG("Collision DIE");
		break;
	default:
		break;
	}
}
