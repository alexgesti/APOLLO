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
	//Camera
	app->render->camera.x = 0;
	if(app->player->position.y >= 0) app->render->camera.y = -app->player->position.y * 1.5;

	if (app->modcontrol->blockx == false)
	{
		//Buoyancy
		if (app->player->position.y > 447)
		{

			if (app->player->position.x < 385 || app->player->position.x > 910)
			{

			}
			else
			{

			}
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

		app->player->position.y += grav;
	}

	LOG("%f %f %f %f %f", app->player->position.x, app->player->position.y, app->player->vel, app->player->acc, grav);

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
