#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"
#include "SString.h"

#include <stdio.h>

class PhysBody;
class b2RevoluteJoint;
enum class ColliderType;

enum class DEBUGKEYSTATE
{
	FPS,
	GRAVITY,
	BOINGFORCE
};

class SceneElement
{
public:
	SceneElement() {

	}
	~SceneElement() {

	}

private:
	SString name;
	PhysBody* pbody;
};

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
	int ball_state = 0;
	bool die = false;
	bool canDebugMode = false;

	p2List<PhysBody*> circles;

	p2List<SceneElement*> SceneElements;

	PhysBody* sensor;
	PhysBody* sensorStart;
	PhysBody* startBloquer;
	PhysBody* pinball;
	PhysBody* spaceShip;
	PhysBody* planet1;
	PhysBody* planet2;
	PhysBody* planet3;
	PhysBody* planet4;
	PhysBody* asteroidsleft;
	PhysBody* asteroidsright;
	PhysBody* ship;
	PhysBody* Ball;
	PhysBody* flipper;
	PhysBody* staticPin;
	PhysBody* flipper2;
	PhysBody* staticPin2;


	b2RevoluteJoint* joint;
	b2RevoluteJoint* joint2;

	bool sensorstart;
	bool sensed;
	int scoreFont = -1;
	int x, y;

	DEBUGKEYSTATE DebugKeyState = DEBUGKEYSTATE::GRAVITY;



	SDL_Texture* ball;
	SDL_Texture* background;
	SDL_Texture* scoreBoard;
	SDL_Texture* trigger;
	SDL_Texture* collision_layout;
	SDL_Texture* framework;
	SDL_Texture* cursorTexture = nullptr;
	SDL_Texture* rightFlipperTexture = nullptr;
	SDL_Texture* leftFlipperTexture = nullptr;

	int flipperx, flippery;
	int flipper2x, flipper2y;
	
	int score, combo;
	ColliderType lastCollider;
	int lastScore = 0, highScore = 0;
	char scoreText[10] = { "\0" };
	char highScoreText[10] = { "\0" };
	char lastScoreText[10] = { "\0" };

	//FPS STUFF TO BLIT
	char FPStext[10] = { "\0" };

	char GRAVITYtext[10] = { "\0" };
	char BOINGFORCEtext[10] = { "\0" };

	bool printLayouts = false;

	Animation background_anim;
	Animation trigger_anim;
	Animation ball_anim;

	int triggerCounter = 0;
	int triggerAnimCounter = 1;

	uint bonus_fx;
	uint boing_fx;
	uint charge_fx;
	uint launch_fx;
	uint damage_fx;
	p2Point<int> ray;
	bool ray_on;
	float desiredvel;


	float boingForce = 4.0f;


	int forcetimer;

	// Pivot 0, 0
	int star_destroyer[8] = {
		4, 49,
		87, 29,
		98, 50,
		87, 70
	};

	// Pivot 0, 0
	int pinballHitbox[52] = {
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
		139, 571,
		//128, 618,
		//234, 618,
		139, 738,
		366, 738,
		//274, 618,
		//377, 618,
		366, 571,
		521, 487,
		521, 140,
		534, 140,
		534, 575,
	};
};
