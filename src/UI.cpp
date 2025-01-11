#include "GuiControl.h"
#include "GuiControlButton.h"
#include "UI.h"
#include "Engine.h"

bool UI::Update(float dt) {
	if (IsActive(GuiClass::MAIN_MENU)) Engine::GetInstance().render.get()->DrawTexture(TitleScreen, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 110, -Engine::GetInstance().render.get()->camera.y / 2);
	if (IsActive(GuiClass::PAUSE)) Engine::GetInstance().render.get()->DrawTexture(OptionsBook, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 185, -Engine::GetInstance().render.get()->camera.y / 2 + 20);

	return true;
}

void UI::Add(GuiClass gui, GuiControl* control) {
	switch (gui)
	{
	case GuiClass::PAUSE:
		pauseMenu.push_back(control);
		break;
	case GuiClass::TPBONFIRE:
		tpMenu.push_back(control);
		break;
	case GuiClass::MAIN_MENU:
		mainMenu.push_back(control);
		break;
	case GuiClass::SETTINGS:
		settings.push_back(control);
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
	case GuiClass::SETTINGS:
		for (auto button : settings) {
			button->Enable();
		}
		settingsActive = true;
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
	case GuiClass::SETTINGS:
		for (auto button : settings) {
			button->Disable();
		}
		settingsActive = false;
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

bool UI::IsActive(GuiClass gui) {
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
	case GuiClass::SETTINGS:
		return settingsActive;
		break;
	}
}