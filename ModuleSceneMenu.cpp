#include "ModuleSceneMenu.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"

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
	menu.speed = 0.2f;
	

	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	menuTexture = App->textures->Load("Assets/Textures/menu.png");
	menuTextureAnim = App->textures->Load("Assets/Textures/menu spritesheet.png");
	iconTexture = App->textures->Load("Assets/Textures/lightsaber.png");

	transitionanim = false;
	playOrExit = true;



	return true;
}

update_status SceneMenu::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		if (playOrExit == true) {
			transitionanim = true;
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
			this->Disable(); 
		}
		
	}
	else {
		menu.Reset();
		App->renderer->Blit(menuTexture, 0, 0);
	}

	if (playOrExit == true && transitionanim == false) {
		App->renderer->Blit(iconTexture, 152, 560);
	}
	else if(transitionanim == false){
		App->renderer->Blit(iconTexture, 152, 615);
	}

	return update_status::UPDATE_CONTINUE;
}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	App->textures->Unload(menuTexture);
	App->textures->Unload(menuTextureAnim);

	return true;
}