#pragma once
#include <vector>
#include "GuiControlButton.h"

class UI {
public:

	UI() {};

	void Add(GuiClass gui, GuiControlButton* button);

	void Active(GuiClass gui);

	void Disable(GuiClass gui);

	int GetSize(GuiClass gui);

	bool IsActive(GuiClass gui);

private:

	std::vector<GuiControlButton*> tpMenu;
	bool tpMenuActive;

	std::vector<GuiControlButton*> pauseMenu;
	bool pauseMenuActive;

	std::vector<GuiControlButton*> mainMenu;
	bool mainMenuActive;
};