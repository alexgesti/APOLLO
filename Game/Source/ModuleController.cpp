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
		app->player->ban = false;
		app->player->active = true;				// Player

		break;

	default:
		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) Reset();

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

bool ModuleController::Reset()
{
	//ModuleController
	blockx = true;
	blocky = false;
	capped = false;
	showcollider = false;
	showbuttons = false;
	currentscene = 1;

	//Player
	app->player->ban = false;
	app->player->dead = false;
	app->player->win = false;
	app->player->surviveinmoon = false;
	app->player->hurtonetime = false;
	app->player->currentanim = &app->player->moveanim;
	app->player->current2anim = &app->player->explosionanim;
	app->player->position.x = app->render->camera.w / 2;
	app->player->position.y = 444;
	app->player->vel.x = 0;
	app->player->vel.y = 0;
	app->player->acc.x = 0;
	app->player->acc.y = 0;
	app->player->acct.x = 0;
	app->player->acct.y = 0;
	app->player->angle = 0;
	app->player->grav.x = 0;
	app->player->grav.y = 9.8f;
	app->player->angle_rotation_value = 0;
	app->player->explosionanim.Reset();

	//Scene
	app->scene->changescene = false;
	app->scene->gameoverpos.x = app->render->camera.w;
	app->scene->gameoveronetimemusic = false;
	app->scene->moononetimesound = false;
	app->scene->onetimesoundexplode = false;
	app->scene->posmoon = app->render->camera.w - 216;
	app->scene->velmoon = 0;

	//SceneEarth
	app->scenearth->flot = 0;
	app->scenearth->gameoverpos.x = app->render->camera.w;
	app->scenearth->drag = 0;
	app->scenearth->winpos.y = app->render->camera.h * 2;
	app->scenearth->watersoundone = false;
	app->scenearth->winonetimemusic = false;
	app->scenearth->gameoveronetimemusic = false;
	app->scenearth->onetimesoundexplode = false;
	app->scenearth->vagua = 0;

	//Audio
	app->audio->PlayMusic("Assets/Audio/Music/pepsiman.ogg", 0);
	return true;
}