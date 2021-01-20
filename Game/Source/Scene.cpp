#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");



	//pillar animation
	/*pillar.PushBack({ 0, 0, 48, 80 });
	pillar.PushBack({ 48, 0, 48, 80 });
	pillar.PushBack({ 96, 0, 48, 80 });
	pillar.loop = true;
	pillar.speed = 0.12f^*/

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
	//spritePillar = app->tex->Load("Assets/Screens/Gameplay/save_point_saving-x64.png");

	backtext = app->tex->Load("Assets/Screens/Gameplay/background.png");
	earthtex = app->tex->Load("Assets/Screens/Gameplay/earth.png");
	moontex = app->tex->Load("Assets/Screens/Gameplay/landplace.png");

	notsceneactived = false;

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
	if (notsceneactived)
	{
		//app->render->camera.x = app->player->position.x + ((app->render->camera.w / 2) - app->player->playerWH / 2);		 
		//app->render->camera.y = app->player->position.y + ((app->render->camera.h / 2) - app->player->playerWH / 2);

		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	//PillarAnim->Update();


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	//SDL_Rect rect = PillarAnim->GetCurrentFrame();
	//app->render->DrawTexture(spritePillar, 2057, 1905, &rect);
	SDL_Rect backrect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(backtext, 0, 0, &backrect);

	for (int i = 0; i < 5; i++)
	{
		app->render->DrawTexture(moontex, app->render->camera.w - 216, 256 * i, NULL);
	}

	app->render->DrawTexture(earthtex, -108, app->render->camera.h - 216 * 2, NULL);

	

	return ret;
}

// Load Scene State (Underconstruction)
bool Scene::LoadState(pugi::xml_node& data)
{
	// Player
	//app->player->position.x = data.child("Player").attribute("x").as_int();
	//app->player->position.y = data.child("Player").attribute("y").as_int();

	// Scene


	return true;
}

// Save Scene State (Underconstruction)
bool Scene::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node playersave = data.append_child("Player");
	pugi::xml_node scenesave = data.append_child("Scene");

	//playersave.append_attribute("x") = app->player->position.x;
	//playersave.append_attribute("y") = app->player->position.y;

	return true;
}

bool Scene::Reset()
{
	app->LoadGameRequest("StartValues.xml");

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
