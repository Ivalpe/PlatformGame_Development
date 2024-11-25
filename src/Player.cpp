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
	dp = DirectionPlayer::RIGHT;
	stPlayer = StatePlayer::IDLE;
	return true;
}

bool Player::Start() {
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	run.LoadAnimations(parameters.child("animations").child("run"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	fall.LoadAnimations(parameters.child("animations").child("fall"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	currentAnimation = &idle;

	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	return true;
}

void Player::SetPosition(Vector2D posPlayer) {
	posPlayer.setX(posPlayer.getX() + texW / 2);
	posPlayer.setY(posPlayer.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(posPlayer.getX()), PIXEL_TO_METERS(posPlayer.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

bool Player::Update(float dt)
{
	if (!isDying) {
		b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
		stPlayer = StatePlayer::IDLE;

		// Move left
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.x = -speed * dt;
			dp = DirectionPlayer::LEFT;
			stPlayer = StatePlayer::RUN;
		}

		// Move right
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.x = speed * dt;
			dp = DirectionPlayer::RIGHT;
			stPlayer = StatePlayer::RUN;
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

		// If the player is jumping, we don't want to apply gravity, we use the current velocity prduced by the jump
		if (isJumping)
			velocity = pbody->body->GetLinearVelocity();

		if (isDying)
			velocity = pbody->body->GetLinearVelocity();

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


		if (dp == DirectionPlayer::LEFT)
			flipType = SDL_FLIP_HORIZONTAL;
		else
			flipType = SDL_FLIP_NONE;

		if (stPlayer == StatePlayer::RUN)
			currentAnimation = &run;
		else if (stPlayer == StatePlayer::IDLE)
			currentAnimation = &idle;
		else if (stPlayer == StatePlayer::JUMP)
			currentAnimation = &jump;
		else if (stPlayer == StatePlayer::FALL)
			currentAnimation = &fall;
		else if (stPlayer == StatePlayer::DIE)
			currentAnimation = &die;


	}else {
		currentAnimation = &die;
	}

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
	case ColliderType::DIE:
		LOG("Collision DIE");
		stPlayer = StatePlayer::DIE;
		isDying = true;
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