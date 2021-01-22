#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneEarth.h"
#include "Player.h"
#include "Audio.h"

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
	app->render->camera.x = 0;
	app->render->camera.y = -app->player->position.y;

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
