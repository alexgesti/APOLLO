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

public:
	fPoint position;	// Position
	float vel;			// Velocity/Speed
	float acc;			// Acceleration
	float angle;		// Angle
	float angle_rotation_value;

	Animation* currentanim;
	Animation* current2anim;

	Animation idleanim;
	Animation moveanim;
	Animation explosionanim;

	bool dead = false;
	bool win = false;
	bool surviveinmoon = false;

private:
	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* water = nullptr;
	SDL_Texture* explosionsheet = nullptr;
	SDL_Texture* wintex;

};

#endif // __SCENE_H__