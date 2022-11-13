#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;


	PhysBody* sensor;
	PhysBody* pinball;
	PhysBody* planet1;
	PhysBody* planet2;
	PhysBody* planet3;
	bool sensed;
	int scoreFont = -1;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* cursorTexture = nullptr;

	Animation background_anim;

	uint bonus_fx;
	uint boing_fx;
	p2Point<int> ray;
	bool ray_on;

	// Pivot 0, 0
	int pinballHitbox[60] = {
		560, 575,
		560, 119,
		558, 92,
		552, 75,
		547, 70,
		541, 65,
		531, 59,
		519, 54,
		506, 52,
		87, 52,
		74, 54,
		64, 58,
		55, 63,
		46, 72,
		42, 80,
		40, 91,
		38, 109,
		38, 515,
		128, 563,
		128, 618,
		234, 618,
		234, 738,
		274, 738,
		274, 618,
		377, 618,
		377, 563,
		521, 487,
		521, 140,
		534, 140,
		534, 575,
	};
};
