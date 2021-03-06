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
	fPoint vel;			// Velocity/Speed
	fPoint acc;			// Player Acceleration
	fPoint acct;		// Total Acceleration
	float angle;		// Angle
	fPoint grav;		// Gravity
	float angle_rotation_value;

	Animation* currentanim;
	Animation* current2anim;

	Animation idleanim;
	Animation moveanim;
	Animation explosionanim;

	uint hurtsound;

	bool ban = false;
	bool dead = false;
	bool win = false;
	bool surviveinmoon = false;
	bool hurtonetime = false;

	float mcohete = 2 * pow(10, 4);		//Masa del player

private:
	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* water = nullptr;
	SDL_Texture* explosionsheet = nullptr;
	SDL_Texture* wintex;

};

#endif // __SCENE_H__