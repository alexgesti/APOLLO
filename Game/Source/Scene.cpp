#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Audio.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

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

	gameoversound = app->audio->LoadFx("Assets/Audio/Music/gameover.ogg");

	gameoverpos.x = app->render->camera.w;

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
	if (app->player->position.x <= 120
		&& app->player->surviveinmoon)
	{
		app->player->angle = 180;
		app->player->position.x = app->render->camera.w / 2;
		app->player->position.y = -145;

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
		app->render->DrawTexture(moontex, app->render->camera.w - 216, 256 * i, NULL);
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
