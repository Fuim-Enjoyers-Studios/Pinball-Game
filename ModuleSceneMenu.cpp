#include "ModuleSceneMenu.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

SceneMenu::SceneMenu(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}


SceneMenu::~SceneMenu()
{

}

// Load assets
bool SceneMenu::Start()
{
	/*LOG("Loading Scene Menu");

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	menu.PushBack({ 0,0,512,384 });
	menu.PushBack({ 0,384,512,384 });
	menu.PushBack({ 512,384,512,384 });
	menu.PushBack({ 512,0,512,384 });
	menu.loop = false;
	menu.speed = 0.1f;

	menuTexture = app->tex->Load(config.child("menu").attribute("texturepath").as_string());

	app->render->camera.x = 0;
	app->render->camera.y = 0;*/
	menu.PushBack({ 0,0,800,800 });
	menu.PushBack({ 1100,0,800,800 });
	menu.PushBack({ 2200,0,800,800 });
	menu.PushBack({ 0,936,800,800 });
	menu.PushBack({ 1100,936,800,800 });
	menu.PushBack({ 2200,936,800,800 });
	menu.PushBack({ 0,1887,800,800 });
	menu.PushBack({ 1100,1887,800,800 });
	

	App->renderer->camera.x = 0;
	App->renderer->camera.y = 0;

	menuTexture = App->textures->Load("Assets/Textures/menu.png");
	menuTextureAnim = App->textures->Load("Assets/Textures/menu spritesheet.png"); 



	return true;
}

update_status SceneMenu::Update()
{
	App->renderer->Blit(menuTexture, 0, 0);




	/*if (appStart) {
		if (iconCounter <= 120) {
			appStart = false;
		}
		iconCounter--;
	}
	else if (iconCounter > 0) {
		menu.Update();
		iconCounter--;
	}
	else {
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			app->fade->FadeToBlack(this, (Module*)app->sceneLevel1, 30);
		}
	}*/

	return update_status::UPDATE_CONTINUE;
}

//bool SceneMenu::PostUpdate()
//{
//	bool ret = true;
//
//	// Draw everything --------------------------------------
//	SDL_Rect rect = menu.GetCurrentFrame();
//	app->render->DrawTexture(menuTexture, 0, 0, &rect);
//
//	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
//		ret = false;
//
//	return ret;
//}

bool SceneMenu::CleanUp()
{
	LOG("Deleting background assets");

	//app->tex->Unload(menuTexture);

	return true;
}