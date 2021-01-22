#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneEarth.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "Player.h"
#include "ModuleController.h"
#include "GameplayHUD.h"


#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);
	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();
	scenearth = new SceneEarth();
	//scenelose = new SceneLose();
	//scenewin = new SceneWin();
	player = new Player();
	modcontrol = new ModuleController();
	//gamehud = new GameplayHUD();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(scenearth);
	AddModule(scene);
	//AddModule(scenelose);
	//AddModule(scenewin);
	AddModule(player);
	AddModule(modcontrol);
	//AddModule(gamehud);

	// Render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		cappedms = configApp.attribute("framerate_cap").as_uint();
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	framecount++;
	lastsecframecount++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frametime.ReadSec();
	frametime.Start();
	delayTimer.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadgamerequested == true) LoadGame();
	if (savegamerequested == true) SaveGame();
    
	FramerateLogic();

	static char title[256];
	sprintf_s(title, 256, "Platformer Game (The Crossing) (FPS: %i / Av.FPS: %.2f / Last Frame Ms: %02u ms / Last dt: %.3f / Play Time: %.3f / Frame Count: %I64u / Vsync: %i / Camera position:%d %d)",
		prevlastsecframecount, averagefps, lastframems, dt, secondssincestartup, framecount, (int)app->render->Vsync, app->render->camera.x, app->render->camera.y);

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) 
		{ 
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc) return args[index];
	else return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest(const char* fileName)
{
	loadgamerequested = true;
	loadedgame.Create(fileName);
}

// ---------------------------------------
void App::SaveGameRequest(const char* fileName) const
{
	savegamerequested = true;
	savedgame.Create(fileName);
}

// ---------------------------------------
// L02: DONE 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(loadedgame.GetString());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", loadedgame.GetString());

		root = data.child("save_state");

		ListItem<Module*> *item = modules.start;
		ret = true;

		while (item != NULL && ret)
		{
			ret = item->data->LoadState(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if (ret)
		{
			LOG("...finished loading");
			savedataexist = true;
		}
		else LOG("...loading process interrupted with error on module %s", item->data->name.GetString());
	}
	else
	{
		savedataexist = false;
		LOG("Could not parse game state xml file %s. pugi error: %s", loadedgame.GetString(), result.description());
	}

	loadgamerequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", savedgame.GetString());

	//xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("save_state");

	ListItem<Module*>* item = modules.start;

	while (item != NULL && ret)
	{
		ret = item->data->SaveState(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if (ret)
	{
		data.save_file(savedgame.GetString());
		LOG("... finished saving");
	}
	else LOG("Save process halted from an error in module %s", item->data->name.GetString());

	data.reset();

	savegamerequested = false;

	return ret;
}

void App::FramerateLogic()
{
	if (lastsecframetime.Read() > 1000)
	{
		lastsecframetime.Start();
		prevlastsecframecount = lastsecframecount;
		lastsecframecount = 0;
	}

	averagefps = float(framecount) / startuptime.ReadSec();
	secondssincestartup = startuptime.ReadSec();
	lastframems = frametime.Read();

	int delayTime = (1000 / cappedms) - lastframems;
	if (delayTime > 0)
	{
		SDL_Delay((Uint32)delayTime);
	}
}


