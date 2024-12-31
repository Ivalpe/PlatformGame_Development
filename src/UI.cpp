#include "GuiControl.h"
#include "GuiControlButton.h"
#include "UI.h"



void UI::Add(GuiClass gui, GuiControlButton* button) {
	switch (gui)
	{
	case GuiClass::PAUSE:
		pauseMenu.push_back(button);
		break;
	case GuiClass::TPBONFIRE:
		tpMenu.push_back(button);
		break;
	case GuiClass::MAIN_MENU:
		mainMenu.push_back(button);
		break;
	}
}

void UI::Active(GuiClass gui) {

	switch (gui) {
	case GuiClass::PAUSE:
		for (auto button : pauseMenu) {
			button->Enable();
		}
		pauseMenuActive = true;
		break;
	case GuiClass::TPBONFIRE:
		for (auto button : tpMenu) {
			button->Enable();
		}
		tpMenuActive = true;
		break;
	case GuiClass::MAIN_MENU:
		for (auto button : mainMenu) {
			button->Enable();
		}
		mainMenuActive = true;
		break;
	}
}

void UI::Disable(GuiClass gui) {
	switch (gui) {
	case GuiClass::PAUSE:
		for (auto button : pauseMenu) {
			button->Disable();
		}
		pauseMenuActive = false;
		break;
	case GuiClass::TPBONFIRE:
		for (auto button : tpMenu) {
			button->Disable();
		}
		tpMenuActive = false;
		break;
	case GuiClass::MAIN_MENU:
		for (auto button : mainMenu) {
			button->Disable();
		}
		mainMenuActive = false;
		break;
	}
}

int UI::GetSize(GuiClass gui) {
	switch (gui) {
	case GuiClass::PAUSE:
		return pauseMenu.size() + 1;
		break;
	case GuiClass::TPBONFIRE:
		return tpMenu.size() + 1;
		break;
	case GuiClass::MAIN_MENU:
		return mainMenu.size() + 1;
		break;
	}
}

bool UI::IsActive(GuiClass gui){
	switch (gui) {
	case GuiClass::PAUSE:
		return pauseMenuActive;
		break;
	case GuiClass::TPBONFIRE:
		return tpMenuActive;
		break;
	case GuiClass::MAIN_MENU:
		return mainMenuActive;
		break;
	}
}
