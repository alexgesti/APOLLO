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
#define G  1.23 * pow(10,-11)

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

	app->player->grav.y = 9.8f;

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

	app->player->grav.x = 0;

	if (app->modcontrol->blockx == false)
	{
		//Buoyancy
		if (app->player->position.y > 460)
		{
			app->player->ban = true;
			if (app->player->position.x < 385 || app->player->position.x > 910)
			{		
				if (watersoundone == false)
				{
					app->audio->PlayFx(watersound);

					watersoundone = true;
				}
				drag = aquadrag * app->player->vel.y;
				vagua = app->player->position.y - 460;
				flot = dagua * vagua * 9.8f;

				if (app->player->position.y > 460 && app->player->position.y < 535 )
				{
					if (app->player->vel.y >= -0.1f && app->player->vel.y <= 0.1f) app->player->win = true;
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
			app->player->grav.y = (G * mearth) / pow((mesosearth + radearth - app->player->position.y * 50), 2);

			if (app->player->vel.y < 0)
				drag = -1 * cofaero * 0.5 * densidad * app->player->vel.y * app->player->vel.y * superficie;
		}
	}
	else
	{
		if (app->player->position.y >= 444)
		{
			app->player->grav.y = 0.0f;
			app->player->acct.y = 0.0f;
			app->player->vel.y = 0.0f;
		}
		else app->player->grav.y = (G*mearth) / pow((mesosearth + radearth - app->player->position.y * 50),2);

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
			drag = 0;
			app->player->acc = { 0, 0 };
			app->player->grav = { 0, 0 };
			app->player->acct = { 0, 0 };
			app->player->vel = { 50, 0 };
		
			app->modcontrol->currentscene = 2;
		}

		if (app->player->vel.y > 0)
			drag = cofaero * 0.5 * densidad * app->player->vel.y * app->player->vel.y * superficie;
		else if (app->player->vel.y < 0)
			drag = -1 * cofaero * 0.5 * densidad * app->player->vel.y * app->player->vel.y * superficie;
	}

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
