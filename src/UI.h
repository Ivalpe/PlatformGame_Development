#pragma once
#include <vector>
#include "GuiControlButton.h"

class UI : public Module {
public:

	UI() {};

	void Add(GuiClass gui, GuiControl* control);

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

private:

	std::vector<GuiControl*> tpMenu;
	std::vector<GuiControl*> pauseMenu;
	std::vector<GuiControl*> mainMenu;
	std::vector<GuiControl*> settings;

	SDL_Texture* OptionsBook;
	SDL_Texture* TitleScreen;

};