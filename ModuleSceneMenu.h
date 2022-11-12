#pragma once
#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:
	//Constructor
	SceneMenu(Application* app, bool start_enabled = true);

	//Destructor
	~SceneMenu();

	// Called when the module is activated
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	//bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* menuTexture = nullptr;
	SDL_Texture* menuTextureAnim = nullptr;
	SDL_Texture* iconTexture = nullptr;
	SDL_Texture* cursorTexture = nullptr;
	Animation* currentMenuAnim;
	Animation menu;


	bool appStart = true;
	bool transitionanim;
	bool playOrExit;

	uint introfx;
	uint musicmenu;
};

#endif