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
	debugMode = false;
	lifes = 5;
	lvl = Level::DISABLED;
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("ix").as_int());
	position.setY(parameters.attribute("iy").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	run.LoadAnimations(parameters.child("animations").child("run"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	fall.LoadAnimations(parameters.child("animations").child("fall"));
	die.LoadAnimations(parameters.child("animations").child("die"));
	dmg.LoadAnimations(parameters.child("animations").child("dmg"));
	currentAnimation = &idle;

	//Load Fx

	pugi::xml_document audioFile;
	pugi::xml_parse_result result = audioFile.load_file("config.xml");

	pdeathSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("pdeathSFX").attribute("path").as_string());

	jumpSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("jumpSFX").attribute("path").as_string());
	landSFX = Engine::GetInstance().audio.get()->LoadFx(audioFile.child("config").child("scene").child("audio").child("fx").child("landSFX").attribute("path").as_string());


	//Player
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

void Player::Respawn() {
	isDying = false;
	die.SetFrame(0);
	deathSoundTimer = 0.0f;
	isJumping = false;
	stPlayer = StatePlayer::IDLE;
	pbody->body->SetType(b2_dynamicBody);
	pbody->body->SetAwake(true);
}

void Player::ChangeDebug() {
	debugMode = debugMode ? false : true;
}

bool Player::Update(float dt)
{
	b2Vec2 velocity;

	if (isDmg) {
		pbody->body->SetType(b2_staticBody);
		if (dmg.HasFinished()) {
			stPlayer = StatePlayer::IDLE;
			isDmg = false;
			currentAnimation = &idle;
			dmg.Reset();
			pbody->body->SetType(b2_dynamicBody);
			pbody->body->SetAwake(true);
		}
	}
	else if (!isDying) {

		if (debugMode) {
			velocity = b2Vec2(0, 0);
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity.y = -speed;
				stPlayer = StatePlayer::RUN;
			}

			// Move right
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity.y = speed;
				stPlayer = StatePlayer::RUN;
			}
		}
		else velocity = b2Vec2(0, -GRAVITY_Y);

		stPlayer = StatePlayer::IDLE;

		// Move left
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.x = -speed;
			dp = DirectionPlayer::LEFT;
			stPlayer = StatePlayer::RUN;
		}

		// Move right
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.x = speed;
			dp = DirectionPlayer::RIGHT;
			stPlayer = StatePlayer::RUN;
		}

		// Walk / Run speed
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) speed = 3.0f;
		else speed = 2.0f;

		//Jump
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {

			hasLanded = false;


			pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
			isJumping = true;

			Engine::GetInstance().audio.get()->PlayFx(jumpSFX);
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
	}
	else {
		pbody->body->SetType(b2_staticBody);
		currentAnimation = &die;

	}


	Engine::GetInstance().render.get()->DrawTexture(texture, flipType, (int)position.getX() + texW / 3, (int)position.getY() - texH / 4, &currentAnimation->GetCurrentFrame());
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
	switch (physA->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		if (isJumping && !hasLanded) {

			Engine::GetInstance().audio.get()->PlayFx(landSFX);
			hasLanded = true;
		}
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM with");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN  with");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER with");
		break;
	default:
		break;
	}

	switch (physB->ctype)
	{
	case ColliderType::GROUND:
		LOG("Collision PLATFORM");
		if (isJumping && !hasLanded) {

			Engine::GetInstance().audio.get()->PlayFx(landSFX);
			hasLanded = true;
		}
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::DIE:
		if (!debugMode) {
			LOG("Collision DIE");
			stPlayer = StatePlayer::DIE;
			isDying = true;
			lifes = 5;
		}
		break;
	case ColliderType::NEW:
		LOG("Collision NEW");
		lvl = Level::NEW;
		break;
	case ColliderType::LOAD:
		LOG("Collision LOAD");
		lvl = Level::LOAD;
		break;
	case ColliderType::ENEMY:
		if (!debugMode && currentAnimation != &dmg) {

			if (lifes == 0) {
				stPlayer = StatePlayer::DIE;

				if (deathSoundTimer <= 0.0f) {
					Engine::GetInstance().audio.get()->PlayFx(pdeathSFX);
					deathSoundTimer = deathSoundCooldown;
				}
				isDying = true;
			}
			else {
				currentAnimation = &dmg;
				isDmg = true;
				lifes--;
			}
		}
		break;
	default:
		break;
	}

	LOG("-----------------------------------------");
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