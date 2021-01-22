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
<<<<<<< Updated upstream
=======
	SDL_Texture* gameovertex;

	fPoint gameoverpos;

	uint gameoversound;
	uint winsound;
	uint explosionsound;

	bool gameoveronetimemusic = false;
	bool winonetimemusic = false;
	bool onetimesoundexplode = false;
	bool change = false;
>>>>>>> Stashed changes

public:

	float grav;

	fPoint winpos;
};

#endif // __SCENEEARTH_H__