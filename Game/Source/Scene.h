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

	bool gameoveronetimemusic = false;

	bool onetimesoundexplode = false;

public:

};

#endif // __SCENE_H__