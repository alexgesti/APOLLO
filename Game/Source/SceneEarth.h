#ifndef __SCENEEARTH_H__
#define __SCENEEARTH_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class SceneEarth : public Module
{
public:

	SceneEarth();

	// Destructor
	virtual ~SceneEarth();

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

	// CollisionOrb
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapOrb);

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

private:

	SDL_Texture* terratext;

	uint gameoversound;
	uint explosionsound;
	uint winsound;
	uint watersound;

	bool watersoundone = false;
	bool winonetimemusic = false;
	bool gameoveronetimemusic = false;
	bool onetimesoundexplode = false;

public:

	SDL_Texture* gameovertex;

	fPoint gameoverpos;

	float grav;
	fPoint winpos;
};

#endif // __SCENEEARTH_H__