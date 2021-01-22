#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneEarth.h"
#include "Player.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

ModuleController::ModuleController() : Module()
{

}

// Destructor
ModuleController::~ModuleController()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void ModuleController::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool ModuleController::Awake(pugi::xml_node& config)
{
	LOG("Loading Module Controller");

	app->win->active = true;			// Windows
	app->input->active = true;			// Input
	app->tex->active = true;			// Texture
	app->audio->active = true;			// Audio
	app->scene->active = true;			// Scene
	app->scenearth->active = true;			// Scene
	app->player->active = true;			// Player
	app->modcontrol->active = true;		// ModControl
	app->render->active = true;			// Render

	currentscene = 1;					// Current Scene

	return true;
}

// Called before the first frame
bool ModuleController::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

bool ModuleController::Update(float dt)
{	
	switch (currentscene)
	{
	case 1:	//Earth
		
		app->scene->active = false;				// Scene
		app->scenearth->active = true;			// SceneEarth
		app->player->active = true;				// Player
	
		break;

	case 2:	//Gameplay
		
		app->scene->active = true;				// Scene
		app->scenearth->active = false;			// SceneEarth
		blockx = false;
		blocky = true;
		app->player->active = true;				// Player

		break;

	default:
		break;
	}

	return true;
}


bool ModuleController::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true && item->data->active == false)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}