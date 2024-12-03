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
	float speed;
	if (inverted) 
		speed = -3; 
	else 
		speed = 3;
	pbody->body->SetLinearVelocity({ speed,0 });

	if (stFireball != StateFireball::DIE) {
		float speed = inverted ? -3.0f : 3.0f;  
		pbody->body->SetLinearVelocity({ speed, 0 });
	}


	if (stFireball == StateFireball::IDLE)
		currentAnimation = &idle;

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH);

	Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Fireball::CleanUp()
{
	return true;
}

void Fireball::SetPosition(Vector2D pos) {
	pos.setX(pos.getX());
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



	LOG("-----------------------------------------");
}

bool Fireball::HasCollision() {
	return col;
}