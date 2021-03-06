#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Scene.h"
#include "SceneEarth.h"
#include "Audio.h"
#include "ModuleController.h"

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

	//explosion
	explosionanim.PushBack({ 0, 0, 144, 144 });		// 1
	explosionanim.PushBack({ 144, 0, 144, 144 });	// 2
	explosionanim.PushBack({ 288, 0, 144, 144 });	// 3
	explosionanim.PushBack({ 432, 0, 144, 144 });	// 4
	explosionanim.PushBack({ 576, 0, 144, 144 });	// 5
	explosionanim.PushBack({ 720, 0, 144, 144 });	// 6
	explosionanim.PushBack({ 864, 0, 144, 144 });	// 7
	explosionanim.PushBack({ 1008, 0, 144, 144 });	// 8
	explosionanim.PushBack({ 1152, 0, 144, 144 });	// 9
	explosionanim.PushBack({ 1296, 0, 144, 144 });	// 10
	explosionanim.PushBack({ 1440, 0, 144, 144 });	// 11
	explosionanim.PushBack({ 0, 0, 0, 0 });			// 12
	explosionanim.loop = false;
	explosionanim.speed = 0.2f;
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
	water = app->tex->Load("Assets/Screens/Gameplay/waterterraria.png");
	explosionsheet = app->tex->Load("Assets/Characters/Player/explosion.png");
	wintex = app->tex->Load("Assets/Screens/Gameplay/GETHARD_win.png");

	hurtsound = app->audio->LoadFx("Assets/Audio/Fx/Characters/hurt.wav");

	currentanim = &moveanim;
	current2anim = &explosionanim;

	position.x = app->render->camera.w / 2;
	position.y = 444;
	vel.x = 0;
	vel.y = 0;
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
	if (dead == false)
	{
		if (app->modcontrol->blocky == false)
		{
			// Movement
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				if (acc.x < 50) acc.x += 0.1f * sin(angle * M_PI / 180);
				if (acc.y < 50) acc.y += 0.1f * cos(angle * M_PI / 180);

				currentanim = &moveanim;
			}
			else
			{
				if (acc.x > 0) acc.x -= 0.5f;
				else if (acc.x < 0) acc.x += 0.5f;
				if (acc.y > 0) acc.y -= 0.5f;
				else if (acc.y < 0) acc.y += 0.5f;

				currentanim = &idleanim;
			}
		}

		acct.y = acc.y - grav.y - app->scenearth->drag/mcohete + app->scenearth->flot/mcohete;
		acct.x = acc.x + grav.x;

		// Verlet
		position.y -= (vel.y * dt + acct.y * dt * dt * 0.5);
		position.x += (vel.x * dt + acct.x * dt * dt * 0.5);

		vel.y += acct.y * dt;
		vel.x += acct.x * dt;

		LOG("\nvelx: %f\nvely: %f\nacctx: %f\naccty: %f\naccx: %f\naccy: %f\ndrag: %f\ngravx: %f\ngravy: %f", vel.x, vel.y, acct.x, acct.y, acc.x, acc.y, app->scenearth->drag, grav.x, grav.y);

		// Rotation
		if (app->modcontrol->blockx == false)
		{
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && ban==false)
			{
				if (angle_rotation_value > -2) angle_rotation_value -= 0.1f;
				else angle_rotation_value = -2;

				if (app->modcontrol->currentscene == 1)
				{
					vel.x = 175;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && ban == false)
			{
				if (angle_rotation_value < 2) angle_rotation_value += 0.1f;
				else angle_rotation_value = 2;

				if (app->modcontrol->currentscene == 1)
				{
					vel.x = -175;
				}
			}
			else
			{
				if (angle_rotation_value < 2.1f && angle_rotation_value >= 0.2f) angle_rotation_value -= 0.1f * dt;
				else if (angle_rotation_value > -2.1f && angle_rotation_value <= -0.2f) angle_rotation_value += 0.1f * dt;
				else if (angle_rotation_value < 0.19f && angle_rotation_value > -0.19f) angle_rotation_value = 0;
			}
		}

		angle += angle_rotation_value;

		if (angle >= 360) angle -= 360;
		else if (angle <= -360) angle += 360;

		if (app->modcontrol->currentscene == 1 && app->scene->changescene)
		{
			if (angle <= 120) angle = 120;
			else if (angle >= 240) angle = 240;
		}

		if (position.x < 0) position.x = 0;
		if (position.x > app->render->camera.w - 65) position.x = app->render->camera.w - 65;

		if (app->scene->changescene) currentanim = &idleanim;
	}

	currentanim->Update();
	if (dead)
	{
		current2anim->Update();

		if (hurtonetime == false)
		{
			app->audio->PlayFx(hurtsound);
			
			hurtonetime = true;
		}
	}

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	if (dead == false)
	{
		SDL_Rect rect = currentanim->GetCurrentFrame();
		app->render->DrawTexture(spritesheet, position.x, position.y, &rect, NULL, angle);
	}
	else
	{
		SDL_Rect rect2 = current2anim->GetCurrentFrame(); 
		if (app->modcontrol->currentscene == 2)
		app->render->DrawTexture(explosionsheet, position.x - 15, position.y, &rect2, NULL, -90);
		else if (app->modcontrol->currentscene == 1)
		app->render->DrawTexture(explosionsheet, position.x - 40, position.y - 10, &rect2, NULL, 0);
	}

	if (app->modcontrol->blockx == false)
	{
		app->render->DrawTexture(water, 0, app->render->camera.y);
	}

	if (win && surviveinmoon)
	{
		SDL_Rect winrect = { 0, 0, app->render->camera.w, app->render->camera.h };
		app->render->DrawTexture(wintex, 0, app->scenearth->winpos.y, &winrect);
	}

	if (explosionanim.FinishedAlready && app->modcontrol->currentscene == 1)
	{
		SDL_Rect gameoverrect = { 0, 0, app->render->camera.w, app->render->camera.h };
		app->render->DrawTexture(app->scenearth->gameovertex, app->scenearth->gameoverpos.x, -app->render->camera.y - 30, &gameoverrect);
	}

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}