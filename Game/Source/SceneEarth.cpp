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
	watersound = app->audio->LoadFx("Assets/Audio/Fx/Characters/water.ogg");

	app->audio->PlayMusic("Assets/Audio/Music/pepsiman.ogg", 0);

	winpos.y = app->render->camera.h * 2;
	gameoverpos.x = app->render->camera.w;

	grav = 9.8f;
	cofaero = 0.37f;
	densidad = 1.225f;
	superficie = 0.57f;

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
	//Camera
	app->render->camera.x = 0;
	if(app->player->position.y >= 0 && app->player->position.y <= 3139) app->render->camera.y = -app->player->position.y * 1.5;

	if (app->modcontrol->blockx == false)
	{
		//Buoyancy
		if (app->player->position.y > 460)
		{
			if (app->player->position.x < 385 || app->player->position.x > 910)
			{		
				if (watersoundone == false)
				{
					app->audio->PlayFx(watersound);

					watersoundone = true;
				}

				if (app->player->acc >= 0)
				{
					app->player->acc -= 0.15f;
				}

				if (app->player->acc <= 0)
				{
					app->player->acc = 0;
					grav += 0.05f;
					app->player->position.y -= grav * dt;
				}

				if (app->player->position.y > 460 && app->player->position.y < 535 && grav > 0)
				{
					grav -= 0.1f;

					if (grav >= -0.5f && grav <= 0.5f) app->player->win = true;
				}

				//Win
				if (app->player->win && app->player->surviveinmoon == true)
				{
					if (winonetimemusic == false)
					{
						app->audio->PlayMusic("", 0);
						app->audio->PlayFx(winsound);

						winonetimemusic = true;
					}

					winpos.y -= 50;

					if (winpos.y <= app->render->camera.h) winpos.y = app->render->camera.h;
				}
				else if (app->player->win && app->player->surviveinmoon == false)
				{
					app->player->dead = true;

					if (onetimesoundexplode == false)
					{
						app->audio->PlayMusic("", 0);
						app->audio->PlayFx(explosionsound);

						onetimesoundexplode = true;
					}
				}
			}
			else
			{
				app->player->dead = true;

				if (onetimesoundexplode == false)
				{
					app->audio->PlayMusic("", 0);
					app->audio->PlayFx(explosionsound);

					onetimesoundexplode = true;
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
		if (app->player->position.y >= 444)
		{
			grav = 0.0f;
			app->player->acct = 0.0f;
			app->player->vel = 0.0f;
		}
		else
		{
			grav = 9.8f;
		}

		//Dead
		if(app->player->position.y > 444 && drag > 12)
		{
			app->player->dead = true;

			if (onetimesoundexplode == false)
			{
				app->audio->PlayMusic("", 0);
				app->audio->PlayFx(explosionsound);

				onetimesoundexplode = true;
			}
		}
		
		//Change scene
		if (app->player->position.y <= -145)
		{
			app->player->angle = 90;
			app->player->position.x = 120;
			app->player->position.y = app->render->camera.h / 2 - 32;
		
			app->modcontrol->currentscene = 2;
		}
	}

	if(app->player->vel < 10 && app->player->vel > -10)
	drag = cofaero * 0.5 * densidad * app->player->vel * app->player->vel * superficie;

	// Lose Screen
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

	return true;
}

// Called each loop iteration
bool SceneEarth::PostUpdate()
{
	bool ret = true;
	
	app->render->DrawTexture(terratext, 0, app->render->camera.y);

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
