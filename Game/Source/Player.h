#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "Animation.h"

#define M_PI 3.14159265358979323846

struct SDL_Texture;

class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

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

	int CollisionPlayer();
	bool CollisionFloorPlayer();

	int CheckCollision(iPoint positionMapPlayer);

public:
	fPoint position;
	iPoint vel;
	float angle;

	Animation* currentanim;
	Animation* currentanimcoll;

	Animation idleanim;
	Animation moveanim;

private:
	SDL_Texture* spritesheet = nullptr;

};

#endif // __SCENE_H__