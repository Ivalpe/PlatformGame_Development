#pragma once
#include <vector>
#include "GuiControlButton.h"

class UI : public Module {
public:

	UI() {};

	void Add(GuiClass gui, GuiControl* control);

	void Remove(GuiClass gui, int id);

	void Active(GuiClass gui);

	void Show(GuiClass gui, bool show);

	void Disable(GuiClass gui);

	bool Update(float dt);

	int GetSize(GuiClass gui);

	bool IsShowing(GuiClass gui);

	void LoadTextures(SDL_Texture* title, SDL_Texture* options) {
		TitleScreen = title;
		OptionsBook = options;
	}

private:

	std::vector<GuiControl*> tpMenu;
	bool tpMenuActive;

	std::vector<GuiControl*> pauseMenu;
	bool pauseMenuActive;

	std::vector<GuiControl*> mainMenu;
	bool mainMenuActive;

	std::vector<GuiControl*> settings;
	bool settingsActive;

	SDL_Texture* OptionsBook;
	SDL_Texture* TitleScreen;

};