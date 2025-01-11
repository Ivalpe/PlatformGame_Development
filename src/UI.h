#pragma once
#include <vector>
#include "GuiControlButton.h"

class UI : public Module {
public:

	UI() {};

	void Add(GuiClass gui, GuiControl* control);

	void Active(GuiClass gui);

	void Disable(GuiClass gui);

	bool Update(float dt);

	int GetSize(GuiClass gui);

	bool IsActive(GuiClass gui);

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