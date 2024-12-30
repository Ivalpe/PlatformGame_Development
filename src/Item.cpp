#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
Item::Item() : Entity(EntityType::ITEM)
{
	
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {

	//initilize textures
	id = parameters.attribute("id").as_int();
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	levelItem = parameters.attribute("level").as_int();
	position.setX(parameters.attribute("x").as_int() * 8);
	position.setY(parameters.attribute("y").as_int() * 8);
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	currentAnimation = &idle;

	
	// L08 TODO 4: Add a physics to an item - initialize the physics body
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	return true;
}

void Item::SetItemType(ItemType it) {
	type = it;
}

bool Item::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, SDL_FLIP_NONE, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());

	currentAnimation->Update();

	b2Vec2 itemPos = pbody->body->GetPosition();
	sensor->body->SetTransform({ itemPos.x, itemPos.y }, 0);

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::SetPosition(Vector2D pos) {
	
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);

	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

bool Item::HasCollision() {
	return col;
}

bool Item::IsCollected() {
	return collected;
}