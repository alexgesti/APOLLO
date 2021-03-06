#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "SceneEarth.h"
#include "Player.h"
#include "Audio.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"
#define G  1.23 * pow(10,-11)

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	backtext = app->tex->Load("Assets/Screens/Gameplay/background.png");
	earthtex = app->tex->Load("Assets/Screens/Gameplay/earth.png");
	moontex = app->tex->Load("Assets/Screens/Gameplay/landplace.png");
	
	gameovertex = app->tex->Load("Assets/Screens/Gameplay/gameover.png");
	explosionsound = app->audio->LoadFx("Assets/Audio/Fx/Characters/bombexplode.wav");
	moonsound = app->audio->LoadFx("Assets/Audio/Fx/Characters/moon.ogg");

	gameoverpos.x = app->render->camera.w;
	posmoon = app->render->camera.w - 216;

	app->player->grav.x = 1.62f;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Limits
	if (app->player->position.y <= -145) app->player->position.y = app->render->camera.h;
	else if (app->player->position.y >= app->render->camera.h) app->player->position.y = -145;

	posmoon += velmoon * dt;

	// Moon aterrizaje
	if (app->player->position.x >= 1125 &&
		app->player->angle <= 275 &&
		app->player->angle >= 265 ||
		app->player->position.x >= 1125 &&
		app->player->angle <= -85 &&
		app->player->angle >= -95)
	{
		if (moononetimesound == false)
		{
			app->audio->PlayFx(moonsound);

			//Velocidad extremadamente peque�a despues del choque, visual no lo puede computar. (7.94*10^-17)
			velmoon = (app->player->vel.x * app->player->mcohete) / memoon;

			moononetimesound = true;
		}

		app->player->position.x = 1125;
		app->player->angle = -90;
		app->player->vel = { 0, 0 };		
		app->player->surviveinmoon = true;
	}
	else if (app->player->position.x >= 1125)
	{
		app->player->position.x = 1125;

		app->player->dead = true;

		if (onetimesoundexplode == false)
		{
			app->audio->PlayMusic("", 0);
			app->audio->PlayFx(explosionsound);

			onetimesoundexplode = true;
		}
	}
	else if (app->player->position.x < 1125 && app->modcontrol->currentscene == 2)
	{
		//Gravity Moon
		app->player->grav.x = (G * memoon) / pow((dgravmoon + radmoon - app->player->position.x * 50), 2);
	}

	if (app->player->explosionanim.FinishedAlready)
	{
		if (gameoveronetimemusic == false)
		{
			app->audio->PlayMusic("Assets/Audio/Music/gameover.ogg");

			gameoveronetimemusic = true;
		}

		gameoverpos.x -= 50;

		if (gameoverpos.x <= 0) gameoverpos.x = 0;
	}

	//Change scene
	if (app->player->position.x <= 120
		&& app->player->position.y >= app->render->camera.h / 2 - 180
		&& app->player->position.y <= app->render->camera.h / 2 + 180)
	{
		app->player->angle = 180;
		app->modcontrol->blocky = true;
		app->player->acc = { 0, 0 };
		app->player->grav = { 0, 0 };
		app->player->acct = { 0, 0 };
		app->player->vel = { 0, -250 };
		app->player->position.x = app->render->camera.w / 2;
		app->player->position.y = -145;
		changescene = true;

		app->modcontrol->currentscene = 1;
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	SDL_Rect backrect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(backtext, 0, 0, &backrect);

	for (int i = 0; i < 5; i++)
	{
		app->render->DrawTexture(moontex, posmoon, 256 * i, NULL);
	}

	app->render->DrawTexture(earthtex, -108, app->render->camera.h - 216 * 2, NULL);

	if (app->player->explosionanim.FinishedAlready)
	{
		SDL_Rect gameoverrect = { 0, 0, app->render->camera.w, app->render->camera.h };
		app->render->DrawTexture(gameovertex, gameoverpos.x, 0, &gameoverrect);
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
