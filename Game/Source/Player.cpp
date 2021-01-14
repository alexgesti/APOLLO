#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Scene.h"
#include "Audio.h"
#include "ModuleController.h"
#include "ScenePause.h"

#include <math.h>
#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	//idle animation
	idleanim.PushBack({ 0, 0, 64, 140 });
	idleanim.loop = false;

	//move animation
	moveanim.PushBack({ 64, 0, 64, 140 });
	moveanim.PushBack({ 128, 0, 64, 140 });
	moveanim.loop = true;
	moveanim.speed = 0.25f;
}

// Destructor
Player::~Player()
{

}

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
	spritesheet = app->tex->Load("Assets/Characters/Player/player.png");

	currentanim = &moveanim;

	position.x = app->render->camera.w / 2;
	position.y = app->render->camera.h / 2;
	vel = { 5,5 };
	angle = 0;

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
	dt *= 100;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= vel.x * cos(angle * M_PI / 180);
		position.x += vel.x * sin(angle * M_PI / 180);
	}

	//Rotation
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		angle -= 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		angle += 5;
	}

	if (position.y <= -145) position.y = app->render->camera.h;

	currentanim->Update();

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentanim->GetCurrentFrame();
	app->render->DrawTexture(spritesheet, position.x, position.y, &rect, NULL, angle);

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
	/*iPoint posMapPlayer[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posMapPlayer[i] = app->map->WorldToMap(-position.x + (int)pointscollision[i][0], -position.y + (int)pointscollision[i][1]);
		if (CheckCollision(posMapPlayer[i]) == 2) life = 0;
		if (CheckCollision(posMapPlayer[i]) == 3) return 1; // Checkpoint 1
		if (CheckCollision(posMapPlayer[i]) == 5) return 4; // Checkpoint 2
		if (CheckCollision(posMapPlayer[i]) == 6) return 5; // Checkpoint 3
		if (CheckCollision(posMapPlayer[i]) == 4)
		{
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->hewin = true;
		}
	}

	if (CheckCollision(posMapPlayer[numnpoints - 1]) == 1) return 2;
	if (CheckCollision(posMapPlayer[numnpoints - 2]) == 1) return 3;*/

	return false;
}

bool Player::CollisionFloorPlayer()
{
	/*iPoint posFloorPlayer[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posFloorPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsfloorcollision[i][0], -position.y + (int)pointsfloorcollision[i][1]);
		if (CheckCollision(posFloorPlayer[i]) == 1) return true;
	}*/

	return false;
}

int Player::CheckCollision(iPoint positionMapPlayer)
{
	//if (app->map->data.layers.At(1)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 1;
	//if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 4;
	//if (app->map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	//if (app->map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 3; // CheckPoint 1
	//if (app->map->data.layers.At(5)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 5; // CheckPoint 2
	//if (app->map->data.layers.At(6)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 6; // CheckPoint 3

	return false;
}