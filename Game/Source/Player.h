#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "Animation.h"

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
	bool gravity = false;
	bool Godmode = false;
	bool dead = false;
	bool LookingR = true;
	bool WasLookingR = true;

	float speedx = 8;
	float speedy = 8;

	//Player dimensions
	int playerWH = 64;
	//int life = 3;

	iPoint position;

	Animation* currentAnim;

	Animation idleRAnim;
	Animation idleLAnim;

	Colliders* playerCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {0, 0}, {64, 0}, {0, 64}, {64, 64} };
	
public:
	int pointsFloorCollision[numnPoints][2] = { {4, 64}, {52, 64}, {4, 64}, {52, 64} };

};

#endif // __SCENE_H__