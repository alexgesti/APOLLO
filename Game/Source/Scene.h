#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	SDL_Texture* backtext;
	SDL_Texture* earthtex;
	SDL_Texture* moontex;
	SDL_Texture* gameovertex;

	fPoint gameoverpos;

	uint gameoversound;
	uint explosionsound;
	uint moonsound;

	bool gameoveronetimemusic = false;
	bool moononetimesound = false;
	bool onetimesoundexplode = false;

	float impulse = 0;
	float radmoon = 0.32f * pow(10,6);
	float memoon = 1.36f * pow(10, 22);

public:

	bool changescene = false;

};

#endif // __SCENE_H__