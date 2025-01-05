#pragma once
#include <vector>
#include "GuiControlButton.h"

class UI {
public:

	UI() {};

	void Add(GuiClass gui, GuiControl* control);

	void Active(GuiClass gui);

	void Disable(GuiClass gui);

	int GetSize(GuiClass gui);

	bool IsActive(GuiClass gui);

private:

	std::vector<GuiControl*> tpMenu;
	bool tpMenuActive;

	std::vector<GuiControl*> pauseMenu;
	bool pauseMenuActive;

	std::vector<GuiControl*> mainMenu;
	bool mainMenuActive;

	std::vector<GuiControl*> settings;
	bool settingsActive;

};