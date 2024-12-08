#include "Fireball.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"

Fireball::Fireball() : Entity(EntityType::FIREBALL) {
}

Fireball::~Fireball() {
}

bool Fireball::Awake() {
	df = DirectionFireball::RIGHT;
	return true;
}

bool Fireball::Start(bool inv) {

	inverted = inv;
	col = false;
	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations


	idle.LoadAnimations(parameters.child("animations").child("idle"));
	explode.LoadAnimations(parameters.child("animations").child("explode"));
	currentAnimation = &idle;

	//load Fx
	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");

	fireball1SFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("fireball1SFX").attribute("path").as_string());
	fireball2SFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("fireball2SFX").attribute("path").as_string());
	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::FIREBALL;
	pbody->listener = this;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	return true;
}

bool Fireball::Update(float dt)
{

	// Add a physics to an item - update the position of the object from the physics.  
	if (stFireball == StateFireball::DIE && currentAnimation->HasFinished()) col = true;
	else if (stFireball == StateFireball::DIE) pbody->body->SetLinearVelocity({ 0, 0 });
	else {
		float speed = inverted ? -5.0f : 5.0f;
		pbody->body->SetLinearVelocity({ speed, 0 });
	}

	if (stFireball == StateFireball::IDLE) currentAnimation = &idle;


	b2Transform pbodyPos = pbody->body->GetTransform();


	if (inverted) position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
	else position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW - texW / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH);



	Engine::GetInstance().render.get()->DrawTexture(texture, inverted ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		Engine::GetInstance().audio.get()->PlayFx(fireball1SFX);
	}
	return true;
}

bool Fireball::CleanUp()
{
	return true;
}

void Fireball::SetPosition(Vector2D pos) {
	float offsetX = inverted ? -texW / 2 : texW / 2;
	pos.setX(pos.getX() + offsetX);
	pos.setY(pos.getY() + texH / 2);

	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Fireball::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}
void Fireball::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (stFireball != StateFireball::DIE && physB->ctype != ColliderType::SENSOR) {
		stFireball = StateFireball::DIE;
		currentAnimation = &explode;
		Engine::GetInstance().audio.get()->PlayFx(fireball2SFX);
		pbody->body->SetLinearVelocity({ 0, 0 });
		LOG("Fireball collided, starting explosion animation.");
	}

	LOG("-----------------------------------------");
}

bool Fireball::HasCollision() {
	return col;
}