#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(100, 100);
	dp = DirectionPlayer::RIGHT;
	stPlayer = StatePlayer::IDLE;
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	for (int i = 0; i < 4; ++i) {
		idle.PushBack({ i * 16, 0, 16, 16 });
	}
	idle.speed = 0.1f;

	// Cargar la animaci�n run (4 cuadros de 16x16, en la segunda fila del sprite)
	for (int i = 0; i < 4; ++i) {
		run.PushBack({ i * 16, 16, 16, 16 });
	}
	run.speed = 0.1f;

	currentAnimation = &idle;


	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);


	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	return true;
}

bool Player::Update(float dt)
{
	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
	stPlayer = StatePlayer::IDLE;

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speed * dt;
		dp = DirectionPlayer::LEFT;

		state = StatePlayer::RUN;

	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed * dt;
		dp = DirectionPlayer::RIGHT;

		state = StatePlayer::RUN;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_IDLE &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_IDLE) {
		state = StatePlayer::IDLE;  // Cambia a estado "idle"

	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speed = 0.16;
	}
	else {
		speed = 0.06;
	}

	//Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
		// Apply an initial upward force
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
	if (isJumping == true)
	{
		velocity = pbody->body->GetLinearVelocity();
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);


	b2Transform pbodyPos = pbody->body->GetTransform();
	if (isJumping == true) {
		if ((int)position.getY() > METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2)
			stPlayer = StatePlayer::JUMP;
		else if ((int)position.getY() < METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2)
			stPlayer = StatePlayer::FALL;
	}
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	if (state == StatePlayer::IDLE) {
		currentAnimation = &idle;
	}
	else if (state == StatePlayer::RUN) {
		currentAnimation = &run;
	}


	if (dp == DirectionPlayer::LEFT)
		flipType = SDL_FLIP_HORIZONTAL;
	else
		flipType = SDL_FLIP_NONE;


	Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		//reset the jump flag when touching the ground
		isJumping = false;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}

}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("End Collision PLATFORM DOWN");
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}