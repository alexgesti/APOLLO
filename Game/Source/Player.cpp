#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
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

		if (CollisionFloorPlayer())
		{
			gravity = false;
			int auxpos = position.y / 16;
			position.y = auxpos * 16;
		}

		if (CollisionPlayer() == 2 && LookingR == true)
		{
			speedx = 0;
			int auxpos = position.x / 8;
			position.x = auxpos * 8;
		}
		else if (CollisionPlayer() == 3 && LookingR == false)
		{
			speedx = 0;
			int auxpos = position.x / 8;
			position.x = auxpos * 8;
		}
		else speedx = 8;

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
	app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}

int Player::CollisionPlayer()
{
	iPoint posMapPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
		if (CheckCollision(posMapPlayer[i]) == 3) return 1;
		}
	if (CheckCollision(posMapPlayer[numnPoints - 1]) == 1) return 2;
	if (CheckCollision(posMapPlayer[numnPoints - 2]) == 1) return 3;

	return false;
}

bool Player::CollisionFloorPlayer()
{
	iPoint posFloorPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posFloorPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsFloorCollision[i][0], -position.y + (int)pointsFloorCollision[i][1]);
		if (CheckCollision(posFloorPlayer[i]) == 1) return true;
	}

	return false;
}

int Player::CheckCollision(iPoint positionMapPlayer)
{
	if (app->map->data.layers.At(1)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 1;
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 4;
	if (app->map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	if (app->map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 3;


	return false;
}