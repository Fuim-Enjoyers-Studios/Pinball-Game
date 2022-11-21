#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModuleFadeToBlack.h"
#include "p2List.h"

#include <ctime>
#include <cstdlib>

#define DEAD 0
#define ALIVE 1

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = NULL;
	ray_on = false;
	sensed = false;

	background_anim.PushBack({ 0, 0, 800, 800 });
	background_anim.PushBack({ 800, 0, 800, 800 });
	background_anim.loop = true;

	trigger_anim.PushBack({ 0, 260 * 0, 260, 260 });
	trigger_anim.PushBack({ 0, 260 * 1, 260, 260 });
	trigger_anim.PushBack({ 0, 260 * 2, 260, 260 });
	trigger_anim.PushBack({ 0, 260 * 3, 260, 260 });
	trigger_anim.PushBack({ 0, 260 * 4, 260, 260 });
	trigger_anim.loop = false;
	trigger_anim.speed = 0.1f;

	ball_anim.PushBack({ 0, 0 * 0, 0, 0 });
	ball_anim.PushBack({ 0, 24 * 0, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 1, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 2, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 3, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 4, 24, 24 });
	ball_anim.loop = false;

	enemys_anim.PushBack({ 0 * 0, 0, 0, 0 });
	enemys_anim.PushBack({ 60 * 0, 0, 60, 60 });
	enemys_anim.PushBack({ 60 * 1, 0, 60, 60 });
	enemys_anim.PushBack({ 60 * 2, 0, 60, 60 });
	enemys_anim.PushBack({ 60 * 3, 0, 60, 60 });
	enemys_anim.PushBack({ 60 * 4, 0, 60, 60 });
	enemys_anim.PushBack({ 60 * 5, 0, 60, 60 });
	enemys_anim.loop = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//shows normal
	//SDL_ShowCursor(true);

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("Assets/Textures/ball.png");
	background = App->textures->Load("Assets/Textures/background.png");
	collision_layout = App->textures->Load("Assets/Textures/collision_layout.png");
	framework = App->textures->Load("Assets/Textures/framework.png");
	cursorTexture = App->textures->Load("Assets/Textures/cursor.png");
	scoreBoard = App->textures->Load("Assets/Textures/score_board.png");
	trigger = App->textures->Load("Assets/Textures/trigger.png");
	rightFlipperTexture = App->textures->Load("Assets/Textures/star_destroyer.png");
	leftFlipperTexture = App->textures->Load("Assets/Textures/star_destroyer_left.png");
	enemys = App->textures->Load("Assets/Textures/enemys.png");
	explosion = App->textures->Load("Assets/Textures/explosion.png");
	mini_explosion = App->textures->Load("Assets/Textures/mini_explosion.png");

	bonus_fx = App->audio->LoadFx("Assets/Audio/bonus.wav");
	boing_fx = App->audio->LoadFx("Assets/Audio/planet_collision1.wav");
	boing2_fx = App->audio->LoadFx("Assets/Audio/planet_collision2.wav");
	charge_fx = App->audio->LoadFx("Assets/Audio/ray_charge.wav");
	launch_fx = App->audio->LoadFx("Assets/Audio/laserball_launch.wav");
	damage_fx = App->audio->LoadFx("Assets/Audio/death_star_explosion.wav");
	death_fx = App->audio->LoadFx("Assets/Audio/death_star_megaexplosion.wav");
	hit_fx = App->audio->LoadFx("Assets/Audio/hard_hit.wav");
	travel1_fx = App->audio->LoadFx("Assets/Audio/spaceship_travel.wav");
	travel2_fx = App->audio->LoadFx("Assets/Audio/spaceship_travel2.wav");


	//LOADS FONTS
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz9012345678" };
	scoreFont = App->fonts->Load("Assets/Fonts/yellowStarWarsFont.png", lookupTable, 1);

	timer = 0;
	desiredvel = -1;


	//creation of a sensor for the win lose condition
	if (sensor == nullptr)
	{
		sensor = App->physics->CreateRectangleSensor(253, 732, 227, 10);
	}
	sensor->body->SetAwake(true);
	sensor->body->SetActive(true);
	sensor->ctype = ColliderType::DEATH;

	sensorstart = false;
	if (sensorStart == nullptr)
	{
		sensorStart = App->physics->CreateRectangleSensor(495, 97, 11, 88);
	}
	sensorStart->body->SetAwake(true);
	sensorStart->body->SetActive(true);
	sensorStart->ctype = ColliderType::SENSORSTART;
	sensorStart->body->SetType(b2BodyType::b2_staticBody);

	if (startBloquer == nullptr)
	{
		startBloquer = App->physics->CreateRectangle(521, 200, 12, 93);
	}
	startBloquer->body->SetAwake(true);
	startBloquer->body->SetActive(true);
	startBloquer->ctype = ColliderType::WALL;
	startBloquer->body->SetType(b2BodyType::b2_staticBody);

	//creation of hitboxes of the PINBALL
	if (pinball == nullptr)
	{
		pinball = App->physics->CreateChain(0, 0, pinballHitbox, 52, true);
	}
	pinball->body->SetAwake(true);
	pinball->body->SetActive(true);
	pinball->ctype = ColliderType::WALL;

	if (spaceShip == nullptr)
	{
		spaceShip = App->physics->CreateRectangle(425, 380, 200, 20);
		spaceShip->SetPosition(425, 380, -10);
		spaceShip->body->SetType(b2BodyType::b2_staticBody);
	}
	spaceShip->body->SetAwake(true);
	spaceShip->body->SetActive(true);
	spaceShip->ctype = ColliderType::NOTBOING;

	//creation of the planet earth collider
	if (planet1 == nullptr) {
		planet1 = App->physics->CreateCircle(125, 322, 50);
		planet1->body->SetType(b2BodyType::b2_staticBody);
		planet1->body->GetFixtureList()->SetRestitution(0.6f);
	}
	planet1->body->SetAwake(true);
	planet1->body->SetActive(true);
	planet1->ctype = ColliderType::BOING;

	//creation of the planet earth collider
	if (planet2 == nullptr) {
		planet2 = App->physics->CreateCircle(374, 238, 40);
		planet2->body->SetType(b2BodyType::b2_staticBody);
		planet2->body->GetFixtureList()->SetRestitution(0.6f);
	}
	planet2->body->SetAwake(true);
	planet2->body->SetActive(true);
	planet2->ctype = ColliderType::BOING;

	//creation of the planet earth collider
	if (planet3 == nullptr) {
		planet3 = App->physics->CreateCircle(125, 125, 25);
		planet3->body->SetType(b2BodyType::b2_staticBody);
		planet3->body->GetFixtureList()->SetRestitution(0.6f);
	}
	planet3->body->SetAwake(true);
	planet3->body->SetActive(true);
	planet3->ctype = ColliderType::BOING;

	if (planet4 == nullptr) {
		planet4 = App->physics->CreateCircle(45,67, 90);
		planet4->body->SetType(b2BodyType::b2_staticBody);
		planet4->body->GetFixtureList()->SetRestitution(0.6f);
	}
	planet4->body->SetAwake(true);
	planet4->body->SetActive(true);
	planet4->ctype = ColliderType::BOING;

	if (asteroidsleft == nullptr) {
		asteroidsleft = App->physics->CreateCircle(195, 225, 16);
		asteroidsleft->body->SetType(b2BodyType::b2_staticBody);
		asteroidsleft->body->GetFixtureList()->SetRestitution(0.6f);
	}
	asteroidsleft->body->SetAwake(true);
	asteroidsleft->body->SetActive(true);
	asteroidsleft->ctype = ColliderType::BOING;

	if (asteroidsright == nullptr) {
		asteroidsright = App->physics->CreateCircle(280, 185, 16);
		asteroidsright->body->SetType(b2BodyType::b2_staticBody);
		asteroidsright->body->GetFixtureList()->SetRestitution(0.6f);
	}
	asteroidsright->body->SetAwake(true);
	asteroidsright->body->SetActive(true);
	asteroidsright->ctype = ColliderType::BOING;

	if (Ball == nullptr) {
		Ball = App->physics->CreateCircle(550, 525, 12);
		Ball->body->GetFixtureList()->SetRestitution(0.6f);
	}
	Ball->body->SetAwake(true);
	Ball->body->SetActive(true);
	Ball->ctype = ColliderType::BALL;
	Ball->listener = this;

	if (enemy == nullptr)
	{
		enemy = App->physics->CreateRectangleSensor(-100, 420, 60, 30);
		enemy->body->SetType(b2BodyType::b2_kinematicBody);
	}
	enemy->body->SetAwake(true);
	enemy->body->SetActive(true);

	//FLIPPERS
	if (joint == nullptr) {
		flipper = App->physics->CreateRectangle(320, 583, 84, 15);
		staticPin = App->physics->CreateRectangle(355, 590, 1, 1);
		flipper->body->SetType(b2BodyType::b2_dynamicBody);
		staticPin->body->SetType(b2BodyType::b2_staticBody);
		flipper->ctype = ColliderType::NOTBOING;
		staticPin->ctype = ColliderType::NOTBOING;

		joint = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(flipper, staticPin, 355, 590);
	}

	joint->SetLimits(-50 * DEGTORAD, 15 * DEGTORAD);
	joint->SetMotorSpeed(20);
	joint->SetMaxMotorTorque(20);
	joint->EnableLimit(true);

	if (joint2 == nullptr) { 
		flipper2 = App->physics->CreateRectangle(186, 583, 84, 15);
		staticPin2 = App->physics->CreateRectangle(150, 590, 1, 1);
		flipper2->body->SetType(b2BodyType::b2_dynamicBody);
		staticPin2->body->SetType(b2BodyType::b2_staticBody);
		flipper2->ctype = ColliderType::NOTBOING;
		staticPin2->ctype = ColliderType::NOTBOING;

		joint2 = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(flipper2, staticPin2, 150, 590);
	}

	joint2->SetLimits(-15 * DEGTORAD, 50 * DEGTORAD);
	joint2->SetMotorSpeed(20);
	joint2->SetMaxMotorTorque(20);
	joint2->EnableLimit(true);

	canDebugMode = true;

	score = 0;
	combo = 0;
	if (lastScore > highScore) highScore = lastScore;

	life = 3;
	isDead = false;

	int enemy_counter = 20;
	bool enemy_active = false;

	App->audio->PlayMusic("Assets/Audio/Star_Wars_Theme.ogg");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(ball);
	App->textures->Unload(background);
	App->textures->Unload(cursorTexture);
	App->textures->Unload(collision_layout);
	App->textures->Unload(framework);
	App->textures->Unload(scoreBoard);
	App->textures->Unload(trigger);
	App->textures->Unload(rightFlipperTexture);
	App->textures->Unload(leftFlipperTexture);
	App->textures->Unload(enemys);

	App->fonts->UnLoad(scoreFont);
	circles.clear();
	trigger_anim.Reset();
	enemys_anim.Reset();

	//deactivates the fixtures in order to dont have collisions anymore
	if (pinball != nullptr) {
		planet1->body->SetActive(false);
		planet2->body->SetActive(false);
		planet3->body->SetActive(false);
		planet4->body->SetActive(false);
		asteroidsleft->body->SetActive(false);
		asteroidsright->body->SetActive(false);
		pinball->body->SetActive(false);
		sensor->body->SetActive(false);
		Ball->body->SetActive(false);
		enemy->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		enemy->body->SetActive(false);
	}

	canDebugMode = false;
	lastScore = score;
	die = true;

	App->audio->PlayMusic("");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//PROVISIONAL PARA CERRAR EL PUTO PROGRAMA DE MIERDA xd lol papu :v:v:v
	if (App->input->GetKey(SDL_SCANCODE_Q))
	{
		return update_status::UPDATE_STOP;
	}


	if (!sensorstart)
	{
		startBloquer->SetPosition(527, 187);
	}
	else if (sensorstart)
	{
		startBloquer->SetPosition(527, 94);
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
	{
		App->fade->FadeToBlack(this, (Module*)App->menu, 60);
	}


	if (App->input->GetKey(SDL_SCANCODE_F2))
	{
		DebugKeyState = DEBUGKEYSTATE::FPS;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3))
	{
		DebugKeyState = DEBUGKEYSTATE::GRAVITY;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F4))
	{
		DebugKeyState = DEBUGKEYSTATE::BOINGFORCE;
	}
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_STATE::KEY_REPEAT && App->physics->debug)
	{
		switch (DebugKeyState)
		{
		case DEBUGKEYSTATE::FPS:
			App->FPS--;
			if (App->FPS <= 1)
			{
				App->FPS = 1;
			}
			break;
		case DEBUGKEYSTATE::GRAVITY:
			App->physics->GRAVITY_Y++;
			if (App->physics->GRAVITY_Y >= 0.0f)
			{
				App->physics->GRAVITY_Y = 0.0f;
			}
			break;
		case DEBUGKEYSTATE::BOINGFORCE:
			boingForce -= 0.2;
			if (boingForce <= 0.2f)
			{
				boingForce = 0.2f;
			}
			break;
		default:
			break;
		}

	}
	else if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_STATE::KEY_REPEAT && App->physics->debug)
	{
		switch (DebugKeyState)
		{
		case DEBUGKEYSTATE::FPS:
			App->FPS++;
			if (App->FPS >= 99)
			{
				App->FPS = 99;
			}
			break;
		case DEBUGKEYSTATE::GRAVITY:
			App->physics->GRAVITY_Y--;
			if (App->physics->GRAVITY_Y <= -100.0f)
			{
				App->physics->GRAVITY_Y = -100.0f;
			}
			break;
		case DEBUGKEYSTATE::BOINGFORCE:
			boingForce += 0.2;
			if (boingForce >= 12.0f)
			{
				boingForce = 12.0f;
			}
			break;
		default:
			break;
		}

	}


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		flipper2->body->SetAngularVelocity(1000 * DEGTORAD * -1);
	}
	else
	{
		flipper2->body->SetAngularVelocity(1000 * DEGTORAD);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		flipper->body->SetAngularVelocity(1000 * DEGTORAD);
	}
	else
	{
		flipper->body->SetAngularVelocity(1000 * DEGTORAD * -1);
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		printLayouts = !printLayouts;
	}

	if (enemy_counter != 0) { enemy_counter--; }

	if (!enemy_active && enemy_counter == 0) { CreateEnemy(); }

	int enemy_x, enemy_y;
	enemy->GetPosition(enemy_x, enemy_y);
	if ((enemy_x > 900 || enemy_x < -100) && enemy_active) {
		enemy_counter = 120;
		enemy_active = false;
	}

	SDL_Rect r = background_anim.GetCurrentFrame();
	App->renderer->Blit(background, 0, 0, &r);

	if (enemy_active) {
		r = enemys_anim.GetCurrentFrame();
		int x, y;
		enemy->GetPosition(x, y);
		App->renderer->Blit(enemys, x+30, y, &r, 1.0f);
	}

	//LAYOUTS PRINTING
	if (printLayouts) {
		App->renderer->Blit(collision_layout, 0, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && sensorstart == false)
	{
		if (ball_state == DEAD) { ball_anim.SetCurrentFrame(triggerAnimCounter + 1); }
		if (triggerCounter == 40) {
			triggerCounter = 0;
			desiredvel -= 1 * triggerAnimCounter;

			trigger_anim.SetCurrentFrame(triggerAnimCounter);
			if (triggerAnimCounter < 4) {
				++triggerAnimCounter;
				App->audio->PlayFx(charge_fx);
			}
			
	
		}
		else { 
			triggerCounter++; 
		}
		r = trigger_anim.GetCurrentFrame();
		App->renderer->Blit(trigger, 302, 547, &r);

	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		b2Vec2 BallInitVelocity = b2Vec2(0.0f, desiredvel);
		Ball->GetPosition(x, y);
	
		if (x > 525 && x < 545 && y >535 && y < 565) {
			Ball->body->ApplyLinearImpulse(BallInitVelocity, Ball->body->GetWorldCenter(), true);
			b2Vec2 temp = Ball->body->GetLinearVelocity();
			if (temp.y < 0) { ball_state = ALIVE; }
			App->audio->PlayFx(launch_fx);
		}


		trigger_anim.Reset();
		triggerAnimCounter = 1;

		desiredvel = -1;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		PhysBody* c1 = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12);
		c1->listener = this;
		c1->ctype = ColliderType::BALL;
		circles.add(c1);
	}

	// Prepare for raycast ------------------------------------------------------

	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	r = { 0, 24 * 4, 24, 24 };
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, &r, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	r = ball_anim.GetCurrentFrame();
	int x, y;
	Ball->GetPosition(x, y);
	App->renderer->Blit(ball, x, y, &r, 1.0f, Ball->GetRotation());

	if (life < 3)
	{
		App->renderer->Blit(explosion, -200, 600);
	}
	if (life < 2)
	{
		App->renderer->Blit(explosion, 100, 600);
	}
	if (life < 1)
	{
		App->renderer->Blit(explosion, -100, 500);
		//App->audio->PlayFx(death_fx);
		App->fade->FadeToBlack(this, (Module*)App->menu, 60);
	}

	//LAYOUTS PRINTING
	if (printLayouts) {
		App->renderer->Blit(framework, 0, 0);
		App->renderer->Blit(scoreBoard, 0, 0);
	}
  
	
	
	//illo, aqui imprime el flipper right

	flipper->GetPosition(flipperx, flippery);
	App->renderer->Blit(rightFlipperTexture, flipperx, flippery, (SDL_Rect*)0, 1.0f, flipper->GetRotation(), 45, 2);

	//illo, aqui imprime el flipper le

	flipper2->GetPosition(flipper2x, flipper2y);
	App->renderer->Blit(leftFlipperTexture, flipper2x, flipper2y, (SDL_Rect*)0, 1.0f, flipper2->GetRotation(), 40, 2);

	if (lastCollider == ColliderType::ENEMY && !enemy_active)
	{
		App->renderer->Blit(mini_explosion, (METERS_TO_PIXELS(enemy->body->GetPosition().x)) - enemy->width, METERS_TO_PIXELS(enemy->body->GetPosition().y) - enemy->height);
	}

	if (score > 9999999) { score = 9999999; }

	//ESCRIBE EL TESTO EN PANTALLA
	sprintf_s(scoreText,10, "%7d", score);
	App->fonts->BlitText(700, 28, scoreFont, "score");
	App->fonts->BlitText(665, 48, scoreFont, scoreText);

	sprintf_s(highScoreText, 10, "%7d", highScore);
	App->fonts->BlitText(625, 68, scoreFont, "highscore");
	App->fonts->BlitText(665, 88, scoreFont, highScoreText);

	sprintf_s(lastScoreText, 10, "%7d", lastScore);
	App->fonts->BlitText(625, 108, scoreFont, "lastscore");
	App->fonts->BlitText(665, 128, scoreFont, lastScoreText);

	sprintf_s(comboText, 10, "%7d", combo);
	App->fonts->BlitText(700, 148, scoreFont, "combo");
	if (combo > 9)
	{
		App->fonts->BlitText(740, 168, scoreFont, "x");
	}
	else
	{
		App->fonts->BlitText(760, 168, scoreFont, "x");
	}
	App->fonts->BlitText(665, 168, scoreFont, comboText);

	if (App->physics->debug)
	{
		//FPS PRITER
		sprintf_s(FPStext, 10, "%7d", App->FPS);
		App->fonts->BlitText(-76, 16, scoreFont, FPStext);
		App->fonts->BlitText(56, 16, scoreFont, "FPS");

		//gravity printer
		sprintf_s(GRAVITYtext, 10, "%7d", (int)App->physics->GRAVITY_Y * -1);
		App->fonts->BlitText(665, 8 + 20 * 9, scoreFont, "gravity");
		App->fonts->BlitText(665, 8 + 20 * 10, scoreFont, GRAVITYtext);

		//boing force printer
		sprintf_s(BOINGFORCEtext, 10, "%7d", (int)boingForce * 10);
		App->fonts->BlitText(685, 8 + 20 * 11, scoreFont, "bounce");
		App->fonts->BlitText(665, 8 + 20 * 12, scoreFont, BOINGFORCEtext);
	}


	


	//CURSOR
	SDL_ShowCursor(false);
	App->renderer->Blit(cursorTexture, App->input->GetMouseX(), App->input->GetMouseY());

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || die)
	{
		if (!die)
		{
			life = 3;
			combo = 0;
			score = 0;
		}
		Ball->SetPosition(546, 563);
		Ball->body->SetLinearVelocity(b2Vec2(0, 0));
		ball_anim.Reset();
		ball_state = DEAD;
		sensorstart = false;
		die = false;

		//Debug balls clear
		p2List_item<PhysBody*>* circleItem;
		circleItem = circles.start;

		while (circleItem != NULL)
		{
			circleItem->data->body->DestroyFixture(circleItem->data->body->GetFixtureList());
			RELEASE(circleItem->data);
			circleItem = circleItem->next;
		}
		circles.clear();

	}
	timer++;
	if (timer > 60) timer = 0;

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	b2Vec2 vec;
	int x, y;

	if (bodyA->ctype == ColliderType::BALL)
	{
		switch (bodyB->ctype)
		{
		case ColliderType::BALL:

			lastCollider = ColliderType::BALL;
			break;
		case ColliderType::WALL:
			//App->audio->PlayFx(bonus_fx);

			lastCollider = ColliderType::WALL;
			break;
		case ColliderType::BOING:
			switch (timer % 3)
			{
			case 0:
				App->audio->PlayFx(boing_fx);
				break;
			case 1:
				App->audio->PlayFx(boing2_fx);
				break;
			case 2:
				App->audio->PlayFx(hit_fx);
				break;
			}
			
			if (lastCollider == ColliderType::BOING) combo++;
			else
			{
				combo = 0;
				combo++;
			}
			score += combo * 100;
			vec = bodyA->body->GetPosition() - bodyB->body->GetPosition();
			vec = b2Vec2(	(bodyA->body->GetPosition().x - bodyB->body->GetPosition().x) * boingForce,
							(bodyA->body->GetPosition().y - bodyB->body->GetPosition().y) * boingForce);
			//BIGGER THE BOINGER, BIGGER THE BOING
			bodyA->body->ApplyLinearImpulse(vec, bodyA->body->GetPosition(), true);

			lastCollider = ColliderType::BOING;
			break;
		case ColliderType::DEATH:
			die = true;
			bodyA->body->SetLinearVelocity(b2Vec2(0, 0));
			

			life--;
			if (life <= 0)
			{
				isDead = true;
				App->audio->PlayFx(death_fx);
			}
			else
			{
				App->audio->PlayFx(damage_fx);
			}

			lastCollider = ColliderType::DEATH;
			break;
		case ColliderType::SENSORSTART:
			sensorstart = true;

			lastCollider = ColliderType::SENSORSTART;
			break;
		case ColliderType::ENEMY:
			if ((sensorstart) && (enemy_active))
			{
				enemy_active = false;
				enemy_counter = 120;
				score *= combo;
				App->audio->PlayFx(hit_fx);
				App->audio->PlayFx(damage_fx);
				enemy->body->SetLinearVelocity(b2Vec2(0, 0));
			}
			

			lastCollider = ColliderType::ENEMY;
			break;
		case ColliderType::NOTBOING:
			switch (timer % 3)
			{
			case 0:
				App->audio->PlayFx(boing_fx);
				break;
			case 1:
				App->audio->PlayFx(boing2_fx);
				break;
			case 2:
				App->audio->PlayFx(hit_fx);
				break;
			}


			lastCollider = ColliderType::WALL;
			break;
		}
	}
	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}

void ModuleSceneIntro::CreateEnemy() {
	srand(time(NULL));

	switch (timer % 2)
	{
	case 0:
		App->audio->PlayFx(travel1_fx);
		break;
	case 1:
		App->audio->PlayFx(travel2_fx);
		break;
	}
	

	bool enemy_direction = true;
	b2Vec2 speed;

	int randNum = (rand() % 2) + 1;

	switch (randNum)
	{
	case 1:
		enemy->SetPosition(-20, 420, enemy->GetRotation());
		speed = b2Vec2(5.0f, 0.0f);
		enemy->body->SetLinearVelocity(speed);
		enemy->body->SetAwake(true);
		enemy->body->SetActive(true);
		enemy->ctype = ColliderType::ENEMY;
		enemy_direction = true;
		break;
	case 2:
		enemy->SetPosition(860, 420, enemy->GetRotation());
		speed = b2Vec2(-5.0f, 0.0f);
		enemy->body->SetLinearVelocity(speed);
		enemy->body->SetAwake(true);
		enemy->body->SetActive(true);
		enemy->ctype = ColliderType::ENEMY;
		enemy_direction = false;
		break;
	default:
		break;
	}

	randNum = (rand() % 3) + 1;

	switch (randNum)
	{
	case 1:
		if(enemy_direction){
			enemys_anim.SetCurrentFrame(1);
		}
		else{
			enemys_anim.SetCurrentFrame(2);
		}
		break;
	case 2:
		if (enemy_direction) {
			enemys_anim.SetCurrentFrame(3);
		}
		else {
			enemys_anim.SetCurrentFrame(4);
		}
		break;
	case 3:
		if (enemy_direction) {
			enemys_anim.SetCurrentFrame(5);
		}
		else {
			enemys_anim.SetCurrentFrame(6);
		}
		break;
	default:
		break;
	}

	enemy_active = true;
}