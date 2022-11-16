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

	ball_anim.PushBack({ 0, 24 * 0, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 1, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 2, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 3, 24, 24 });
	ball_anim.PushBack({ 0, 24 * 4, 24, 24 });
	ball_anim.loop = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	b2RevoluteJoint* joint;
	b2RevoluteJoint* joint2;

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

	bonus_fx = App->audio->LoadFx("Assets/Audio/bonus.wav");
	boing_fx = App->audio->LoadFx("Assets/Audio/boingSound.wav");

	//LOADS FONTS
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz9012345678" };
	scoreFont = App->fonts->Load("Assets/Fonts/yellowStarWarsFont.png", lookupTable, 1);

	forcetimer = 0;
	desiredvel = -1;

	//creation of a sensor for the win lose condition
	if (sensor == nullptr)
	{
		sensor = App->physics->CreateRectangleSensor(254, 712, 40, 50);
	}
	sensor->body->SetAwake(true);
	sensor->body->SetActive(true);
	sensor->ctype = ColliderType::DEATH;

	//creation of hitboxes of the PINBALL
	if (pinball == nullptr)
	{
		pinball = App->physics->CreateChain(0, 0, pinballHitbox, 60, true);
	}
	pinball->body->SetAwake(true);
	pinball->body->SetActive(true);
	pinball->ctype = ColliderType::WALL;

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

	//FLIPPERS
	flipper = App->physics->CreateChain(270, 530, star_destroyer, 8);
	flipper->body->SetTransform({ 5.38,11.36 }, -0.21);
	staticPin = App->physics->CreateRectangle(355, 600, 2, 2);
	flipper->body->SetType(b2BodyType::b2_dynamicBody);
	staticPin->body->SetType(b2BodyType::b2_staticBody);

	joint = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(flipper, staticPin, 270, 530);

	joint->SetLimits(-30 * DEGTORAD, 30 * DEGTORAD);
	joint->SetMotorSpeed(-20);
	joint->SetMaxMotorTorque(20);

	flipper2 = App->physics->CreateChain(270, 530, star_destroyer, 8);
	flipper2->body->SetTransform({ 4.35,13.27 }, 3.34);
	staticPin2 = App->physics->CreateRectangle(150, 600, 2, 2);
	flipper2->body->SetType(b2BodyType::b2_dynamicBody);
	staticPin2->body->SetType(b2BodyType::b2_staticBody);

	joint2 = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(flipper2, staticPin2, 270, 530);

	joint2->SetLimits(-30 * DEGTORAD, 30 * DEGTORAD);
	joint2->SetMotorSpeed(-20);
	joint2->SetMaxMotorTorque(20);




	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(ball);
	App->textures->Unload(background);
	App->textures->Unload(cursorTexture);
	App->fonts->UnLoad(scoreFont);
	circles.clear();
	trigger_anim.Reset();

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

	}

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
	{
		App->fade->FadeToBlack(this, (Module*)App->menu, 60);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT))
	{
		//flipper2->body->SetLinearVelocity({ 0,1000 });
		//flipper2->body->ApplyAngularImpulse(3000,true);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
	{
		//flipper->SetPosition();
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		if(printLayouts){ printLayouts = false; }
		else{ printLayouts = true; }
	}

	SDL_Rect r = background_anim.GetCurrentFrame();
	App->renderer->Blit(background, 0, 0, &r);

	//LAYOUTS PRINTING
	if (printLayouts) {
		App->renderer->Blit(collision_layout, 0, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{

		if (triggerCounter == 40) {
			triggerCounter = 0;
			desiredvel -= 1 * triggerAnimCounter;

			trigger_anim.SetCurrentFrame(triggerAnimCounter);
			if (ball_state == DEAD) { ball_anim.SetCurrentFrame(triggerAnimCounter); }
			if (triggerAnimCounter < 4) {
				++triggerAnimCounter;
			}
			
	
		}
		else { 
			triggerCounter++; 
		}
		r = trigger_anim.GetCurrentFrame();
		App->renderer->Blit(trigger, 302, 547, &r);

	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		b2Vec2 BallInitVelocity = b2Vec2(0.0f, desiredvel);
		Ball->GetPosition(x, y);
	
		if (x > 525 && x < 545 && y >535 && y < 565) {
			Ball->body->ApplyLinearImpulse(BallInitVelocity, Ball->body->GetWorldCenter(), true);
			b2Vec2 temp = Ball->body->GetLinearVelocity();
			if (temp.y < 0) { ball_state = ALIVE; }
		}


		trigger_anim.Reset();
		triggerAnimCounter = 1;

		desiredvel = -1;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->ctype = ColliderType::BALL;
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

	//LAYOUTS PRINTING
	if (printLayouts) {
		App->renderer->Blit(framework, 0, 0);
		App->renderer->Blit(scoreBoard, 0, 0);
	}
  
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || die)
	{
		Ball->SetPosition(546, 563);
		Ball->body->SetLinearVelocity(b2Vec2(0,0));
		circles.clear();
		ball_anim.Reset();
		ball_state = DEAD;
		die = false;
	}
	
	//ESCRIBE EL TESTO EN PANTALLA
	App->fonts->BlitText(600, 248, scoreFont, "Tu MAMA LA MAMA");
  
	//CURSOR
	SDL_ShowCursor(false);
	App->renderer->Blit(cursorTexture, App->input->GetMouseX(), App->input->GetMouseY());

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyA->ctype == ColliderType::BALL)
	{
		switch (bodyB->ctype)
		{
		case ColliderType::BALL:
			break;
		case ColliderType::WALL:
			//App->audio->PlayFx(bonus_fx);
			break;
		case ColliderType::BOING:
			App->audio->PlayFx(boing_fx);
			//vec = b2Vec2(bodyB->width - bodyA->width, bodyB->height - bodyA->height);
			bodyA->body->SetLinearVelocity(b2Vec2(10, 10));
			break;
		case ColliderType::DEATH:
			die = true;
			bodyA->body->SetLinearVelocity(b2Vec2(0, 0));
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
