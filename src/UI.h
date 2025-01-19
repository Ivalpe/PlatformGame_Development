#pragma once
#include <vector>
#include "GuiControlButton.h"
#include "Module.h"
#include "Player.h"
#include "Power.h"
#include "Enemy.h"
#include "Item.h"
#include "Bonfire.h"
#include "Poison.h"
#include "GuiControlButton.h"
#include "UI.h"
#include "Npc.h"

class UI : public Module {
public:

	UI() {};

	void Add(GuiClass gui, GuiControl* control);

	bool GetFullscreen();

	void Remove(GuiClass gui, int id);

	void Active(GuiClass gui, int id);

	void Show(GuiClass gui, bool show);

	void Disable(GuiClass gui, int id);

	bool Update(float dt);

	int GetSize(GuiClass gui);

	bool IsShowing(GuiClass gui);

	void LoadTextures(SDL_Texture* title, SDL_Texture* options) {
		TitleScreen = title;
		OptionsBook = options;
	}

	bool CleanUp();

private:

	std::vector<GuiControl*> tpMenu;
	std::vector<GuiControl*> pauseMenu;
	std::vector<GuiControl*> mainMenu;
	std::vector<GuiControl*> settings;

	SDL_Texture* OptionsBook;
	SDL_Texture* TitleScreen;

};