#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "SceneLose.h"
#include "SceneWin.h"
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

	app->win->active = true;		// Windows
	app->input->active = true;		// Input
	app->tex->active = true;		// Texture
	app->scene->active = true;		// Scene
	//app->sceneIntro->active = true;	// SceneIntro
	//app->sceneLogo->active = true;	// SceneLogo
	//app->sceneLose->active = true;	// SceneLose
	//app->sceneWin->active = true;	// SceneWin
	app->player->active = true;		// Player
	app->modcontrol->active = true;	// ModControl
	app->render->active = true;		// Render

	currentscene = 2;				// Current Scene
	
	app->SaveGameRequest("StartValues.xml");

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
	// DEBUG KEYS
	if (app->scene->NotSceneActived)
	{
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			app->SaveGameRequest("save_game.xml");

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			app->LoadGameRequest("save_game.xml");

		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			if (app->player->Godmode == false)
				app->player->Godmode = true;
			else if (app->player->Godmode == true)
				app->player->Godmode = false;
		}
	}

	/*if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		currentscene = 0;
		app->sceneLogo->Reset();
	}
	if (app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		currentscene = 1;
		app->sceneLogo->MusicOn = true;
		app->sceneIntro->OneTimeOnly = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		currentscene = 2;

		app->scene->Reset();
	}
	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) currentscene = 3;
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) currentscene = 4;*/


	/*if (app->player->life <= 0 && (app->player->deadLAnim.FinishedAlready || app->player->deadRAnim.FinishedAlready))
	{
		app->player->deadRAnim.Reset();
		app->player->deadLAnim.Reset();
		currentscene = 3;
	}*/
	
	switch (currentscene)
	{
	case 0:	//Logo

		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		//app->sceneIntro->active = false;		// SceneIntro
		//app->sceneLogo->active = true;			// SceneLogo
		//app->sceneLose->active = false;			// SceneLose
		//app->sceneWin->active = false;			// SceneWin

		app->render->SetBackgroundColor(black);	

		break;

	case 1:	//Intro

		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		//app->sceneIntro->active = true;			// SceneIntro
		//app->sceneLogo->active = false;			// SceneLogo
		//app->sceneLose->active = false;			// SceneLose
		//app->sceneWin->active = false;			// SceneWin

		break;

	case 2:	//Gameplay

		app->scene->active = true;				// Scene
		app->scene->NotSceneActived = true;		// SceneCamera
		app->player->active = true;				// Player
		//app->sceneIntro->active = false;		// SceneIntro
		//app->sceneLogo->active = false;			// SceneLogo
		//app->sceneLose->active = false;			// SceneLose
		//app->sceneWin->active = false;			// SceneWin

		app->render->SetBackgroundColor(black);

		break;

	case 3:	//Ending

		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		//app->sceneIntro->active = false;		// SceneIntro
		//app->sceneLogo->active = false;			// SceneLogo
		//app->sceneLose->active = true;			// SceneLose
		//app->sceneWin->active = false;			// SceneWin

		break;

	case 4: //Win

		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		//app->sceneIntro->active = false;		// SceneIntro
		//app->sceneLogo->active = false;			// SceneLogo
		//app->sceneLose->active = false;			// SceneLose
		//app->sceneWin->active = true;			// SceneWin

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