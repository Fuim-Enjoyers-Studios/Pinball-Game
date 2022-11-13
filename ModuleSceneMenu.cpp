#include "ModuleSceneMenu.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleSceneIntro.h"
#include "Globals.h"

SceneMenu::SceneMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}


SceneMenu::~SceneMenu()
{

}

// Load assets
bool SceneMenu::Start()
{
	menu.PushBack({ 0,0,800,800 });
	menu.PushBack({ 1100,0,800,800 });
	menu.PushBack({ 2200,0,800,800 });
	menu.PushBack({ 0,936,800,800 });
	menu.PushBack({ 1100,936,800,800 });
	menu.PushBack({ 2200,936,800,800 });
	menu.PushBack({ 0,1887,800,800 });
	menu.PushBack({ 1100,1887,800,800 });
	menu.speed = 0.1f;
	menu.loop = false;

	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	menuTexture = App->textures->Load("Assets/Textures/menu.png");
	menuTextureAnim = App->textures->Load("Assets/Textures/menu spritesheet.png");
	iconTexture = App->textures->Load("Assets/Textures/lightsaber.png");
	cursorTexture = App->textures->Load("Assets/Textures/cursor.png");
	introfx = App->audio->LoadFx("Assets/Audio/xwing.wav");

	transitionanim = false;
	playOrExit = true;

	return true;
}

update_status SceneMenu::Update()
{

	//click start with mouse
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > 300 &&
		App->input->GetMouseX() < 500 &&
		App->input->GetMouseY() > 563 &&
		App->input->GetMouseY() < 611)
	{
		App->audio->PlayFx(introfx);
		transitionanim = true;
		//App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
	}
	//click exit with mouse
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN &&
		App->input->GetMouseX() > 330 &&
		App->input->GetMouseX() < 470 &&
		App->input->GetMouseY() > 611 &&
		App->input->GetMouseY() < 662)
	{
		return update_status::UPDATE_STOP;
	}



	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||
		App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		if (playOrExit == true) {
			transitionanim = true;
			App->audio->PlayFx(introfx);
		}
		else {
			return update_status::UPDATE_STOP;
		}	
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		playOrExit = true;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		playOrExit = false;
	}
	
	if (transitionanim == true) {
		
		currentMenuAnim = &menu;
		currentMenuAnim->Update();
		SDL_Rect rect = currentMenuAnim->GetCurrentFrame();
		App->renderer->Blit(menuTextureAnim, 0, 0, &rect);
		if (menu.GetCurrentFrameint() == 7) {
			App->fade->FadeToBlack(this, (Module*)App->scene_intro, 60);
			menu.SetCurrentFrame(7);
		}
		
	}
	else {
		menu.Reset();
		App->renderer->Blit(menuTexture, 0, 0);
	}

	if (!transitionanim)
	{

		if (App->input->GetMouseX() > 300 &&
			App->input->GetMouseX() < 500 &&
			App->input->GetMouseY() > 563 &&
			App->input->GetMouseY() < 611)
		{
			App->renderer->Blit(iconTexture, 152, 560);
			playOrExit = true;
		}
		else if (App->input->GetMouseX() > 330 &&
			App->input->GetMouseX() < 470 &&
			App->input->GetMouseY() > 611 &&
			App->input->GetMouseY() < 662)
		{
			App->renderer->Blit(iconTexture, 152, 615);
			playOrExit = false;
		}
		else if (playOrExit == true) 
		{
			App->renderer->Blit(iconTexture, 152, 560);
		}
		else 
		{
			App->renderer->Blit(iconTexture, 152, 615);
		}
	}

	SDL_ShowCursor(false);


	//IF CURSOR GOES OUT THE WINDOW IT WILL RETURN FROM THE OTHER SIDE
	// 
	//if (App->input->GetMouseX() >= SCREEN_WIDTH - 1)
	//{
	//	POINT p;
	//	GetCursorPos(&p);
	//	SetCursorPos(p.x - (SCREEN_WIDTH - 1), p.y);
	//}
	//if (App->input->GetMouseX() <= 1)
	//{
	//	POINT p;
	//	GetCursorPos(&p);
	//	SetCursorPos(p.x + (SCREEN_WIDTH - 1), p.y);
	//}
	//if (App->input->GetMouseY() >= SCREEN_HEIGHT - 1)
	//{
	//	POINT p;
	//	GetCursorPos(&p);
	//	SetCursorPos(p.x, p.y - (SCREEN_HEIGHT - 1));
	//}
	//if (App->input->GetMouseY() <= 1)
	//{
	//	POINT p;
	//	GetCursorPos(&p);
	//	SetCursorPos(p.x, p.y + (SCREEN_HEIGHT - 1));
	//}

	App->renderer->Blit(cursorTexture, App->input->GetMouseX(), App->input->GetMouseY());

	return update_status::UPDATE_CONTINUE;
}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	App->textures->Unload(menuTexture);
	App->textures->Unload(menuTextureAnim);
	App->textures->Unload(iconTexture);
	App->textures->Unload(cursorTexture);

	return true;
}