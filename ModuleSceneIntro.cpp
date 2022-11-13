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

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = box = rick = NULL;
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

	//shows normal
	//SDL_ShowCursor(true);

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball = App->textures->Load("Assets/Textures/ball.png"); 
	box = App->textures->Load("Assets/Textures/crate.png");
	rick = App->textures->Load("Assets/Textures/rick_head.png");
	background = App->textures->Load("Assets/Textures/background_spritesheet.png");
	cursorTexture = App->textures->Load("Assets/Textures/cursor.png");
	scoreBoard = App->textures->Load("Assets/Textures/score_board.png");
	trigger = App->textures->Load("Assets/Textures/trigger.png");

	bonus_fx = App->audio->LoadFx("Assets/Audio/bonus.wav");
	boing_fx = App->audio->LoadFx("Assets/Audio/boingSound.wav");

	//LOADS FONTS
	char lookupTable[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz9012345678" };
	scoreFont = App->fonts->Load("Assets/Fonts/yellowStarWarsFont.png", lookupTable, 1);

	//creation of a sensor for the win lose condition
	if (sensor==nullptr)
	{
		sensor = App->physics->CreateRectangleSensor(254, 712, 40, 50);
	}
	sensor->body->SetAwake(true);
	sensor->body->SetActive(true);
	sensor->ctype = ColliderType::SENSOR;

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
		planet1 = App->physics->CreateCircle(125,325,50);
		planet1->body->SetType(b2BodyType::b2_staticBody);
		planet1->body->GetFixtureList()->SetRestitution(0.6f);
	}
	planet1->body->SetAwake(true);
	planet1->body->SetActive(true);
	planet1->ctype = ColliderType::BOING;

	//creation of the planet earth collider
	if (planet2 == nullptr) {
		planet2 = App->physics->CreateCircle(375, 240, 40);
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



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(ball);
	App->textures->Unload(box);
	App->textures->Unload(rick);
	App->textures->Unload(background);
	App->textures->Unload(cursorTexture);
	App->fonts->UnLoad(scoreFont);
	ricks.clear();
	circles.clear();
	boxes.clear();
	trigger_anim.Reset();

	//deactivates the fixtures in order to dont have collisions anymore
	if (pinball != nullptr) {
		planet1->body->SetActive(false);
		planet2->body->SetActive(false);
		planet3->body->SetActive(false);
		pinball->body->SetActive(false);
		sensor->body->SetActive(false);
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

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		background_anim.Update();
		ball_anim.Update();
	}

	SDL_Rect r = background_anim.GetCurrentFrame();
	App->renderer->Blit(background, 0, 0, &r);

	if (triggerCounter==20) {
		trigger_anim.Update();
		triggerCounter = 0;
	}
	else { triggerCounter++; }
	r = trigger_anim.GetCurrentFrame();
	App->renderer->Blit(trigger, 302, 547, &r);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
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
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, &r, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//ESCRIBE EL TEXTO EN PANTALLA
	App->fonts->BlitText(0, 248, scoreFont, "TU MAMA LA MAMA");

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
			b2Vec2 vec = b2Vec2(bodyB->width - bodyA->width, bodyB->height - bodyA->height);

			bodyA->body->SetLinearVelocity(vec);
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
