#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneEarth.h"
#include "Player.h"
#include "Audio.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

SceneEarth::SceneEarth() : Module()
{
	name.Create("scenearth");
}

// Destructor
SceneEarth::~SceneEarth()
{}

// Called before render is available
bool SceneEarth::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneEarth::Start()
{
	terratext = app->tex->Load("Assets/Screens/Gameplay/terrariacaptura.png");

	gameovertex = app->tex->Load("Assets/Screens/Gameplay/gameover.png");
	gameoversound = app->audio->LoadFx("Assets/Audio/Music/gameover.ogg");
	explosionsound = app->audio->LoadFx("Assets/Audio/Fx/Characters/bombexplode.wav");
	winsound = app->audio->LoadFx("Assets/Audio/Music/win.ogg");

	winpos.y = app->render->camera.h * 2;
	gameoverpos.x = app->render->camera.w;

	grav = 0;

	return true;
}

// Called each loop iteration
bool SceneEarth::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneEarth::Update(float dt)
{ 
	dt *= 100;

	//Camera
	app->render->camera.x = 0;
	if(app->player->position.y >= 0 && app->player->position.y <= 3139) app->render->camera.y = -app->player->position.y * 1.5;

	if (app->modcontrol->blockx == false)
	{
		//Buoyancy
		if (app->player->position.y > 447)
		{
			if (app->player->position.x < 385 || app->player->position.x > 910)
			{				
				if (app->player->acc > 0 && change == false)
				{
					app->player->acc -= 0.1f;
					grav += 0.25f;
				}
				if (app->player->acc <= 0)
				{
					change = true;
					app->player->position.y -= grav * dt;
					grav -= 0.15f;
				}
			}
			else
			{
				app->player->dead = true;

				if (onetimesoundexplode == false)
				{
					app->audio->PlayFx(explosionsound);

					onetimesoundexplode = true;
				}

				if (app->player->explosionanim.FinishedAlready)
				{
					if (gameoveronetimemusic == false)
					{
						app->audio->PlayFx(gameoversound);

						gameoveronetimemusic = true;
					}

					gameoverpos.x -= 50;

					if (gameoverpos.x <= 0) gameoverpos.x = 0;
				}
			}
		}
		else 
		{
			app->player->vel = 1;
			if(app->player->acc < 7.0f) app->player->acc += 0.05f;
		}
	}
	else
	{
		//Gravity
		if (app->player->acc >= 1.4f && app->player->vel < 3.0f) app->player->vel += 0.05f;
		if (app->player->acc < 1.4f && app->player->vel > 1.0f)	app->player->vel -= 0.1f;

		if (app->player->position.y >= 444)
		{
			grav = 0.0f;
		}

		app->player->position.y += grav * dt;

		if(grav >= 1)
		{
			app->player->dead = true;

			if (onetimesoundexplode == false)
			{
				app->audio->PlayFx(explosionsound);

				onetimesoundexplode = true;
			}
		}

		if (app->player->explosionanim.FinishedAlready)
		{
			if (gameoveronetimemusic == false)
			{
				app->audio->PlayFx(gameoversound);

				gameoveronetimemusic = true;
			}

			gameoverpos.x -= 50;

			if (gameoverpos.x <= 0) gameoverpos.x = 0;
		}
		
		//Change scene
		if (app->player->position.y <= -145 && app->player->surviveinmoon == false)
		{
			app->player->angle = 90;
			app->player->position.x = 120;
			app->player->position.y = app->render->camera.h / 2 - 32;
		
			app->modcontrol->currentscene = 2;
		}

		//Win
		if (app->player->win)
		{
			if (winonetimemusic == false)
			{
				app->audio->PlayFx(winsound);

				winonetimemusic = true;
			}

			winpos.y -= 50;

			if (winpos.y <= app->render->camera.h) winpos.y = app->render->camera.h;
		}
	}

	LOG("%f %f %f", app->player->acc, app->player->vel, grav);

	return true;
}

// Called each loop iteration
bool SceneEarth::PostUpdate()
{
	bool ret = true;
	
	app->render->DrawTexture(terratext, 0, app->render->camera.y);

	if (app->player->explosionanim.FinishedAlready)
	{
		SDL_Rect gameoverrect = { 0, 0, app->render->camera.w, app->render->camera.h };
		app->render->DrawTexture(gameovertex, gameoverpos.x, -app->render->camera.y - 30, &gameoverrect);
	}

	return ret;
}

bool SceneEarth::Reset()
{
	app->LoadGameRequest("StartValues.xml");

	return true;
}

// Called before quitting
bool SceneEarth::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
