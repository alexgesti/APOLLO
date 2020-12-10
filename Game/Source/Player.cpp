#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Scene.h"
#include "Audio.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	//idle rigth animation
	/*idleRAnim.PushBack({ 0, 0, 64, 64 });
	idleRAnim.PushBack({ 64, 0, 64, 64 });
	idleRAnim.PushBack({ 128, 0, 64, 64 });
	idleRAnim.PushBack({ 192, 0, 64, 64 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.15f;*/
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	//Load texture
	//spriteSheet = app->tex->Load("Assets/Characters/Hero/herochar_spriteSheet.png");

	currentAnim = &idleRAnim;

	position.x = 0;
	position.y = 0;

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (Godmode == false)
	{
		//Gravity
		if (gravity == true && dead == false)
		{
			position.y += speedy;
		}

		//Idle	
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			|| (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			&& dead == false)
		{
			if (LookingR) currentAnim = &idleRAnim;
			else currentAnim = &idleLAnim;
		}

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& dead == false)
		{
			position.x -= speedx;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE 
			&& dead == false)
		{
			position.x += speedx;

			LookingR = true;
		}

		//Jump
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN
			&& dead == false)
		{
			gravity = false;
		}

		//Die
		/*if (life <= 0)
		{
			dead = true;

			if (oncesound == false)
			{
				oncesound = true;
				app->audio->PlayFx(deathFx);
			}

			if (LookingR) currentAnim = &deadRAnim;
			else currentAnim = &deadLAnim;
		}*/

	}

	//Godmode
	if (Godmode == true)
	{
		gravity = false;
		speedx = 8;

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			position.x -= speedx;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			position.x += speedx;

			LookingR = true;
		}

		//Mov up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
			position.y -= speedx;

		//Mov down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			position.y += speedx;
	}

	currentAnim->Update();

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(spriteSheet, position.x, position.y, &rect);

	return ret;
}

// Collision
bool Player::CheckCollisionRec(iPoint positionMapPlayer, iPoint positionSuelo)
{
	/*if ((positionMapPlayer.x < (positionSuelo.x + )) && ((positionMapPlayer.x + ) > positionSuelo.x) &&
		(positionMapPlayer.y < (positionSuelo.y + )) && ((positionMapPlayer.y + ) > positionSuelo.y)) return true;*/

	// Base utilizable, recomendable hacer script que sea "fisics" o "colliders" y tenga esta base por posicion, ahorra muchos problemas futuros

	return false;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}