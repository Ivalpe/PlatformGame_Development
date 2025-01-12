#include "GuiControl.h"
#include "GuiControlButton.h"
#include "UI.h"
#include "Engine.h"

bool UI::Update(float dt) {
	if (IsShowing(GuiClass::MAIN_MENU)) Engine::GetInstance().render.get()->DrawTexture(TitleScreen, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 110, -Engine::GetInstance().render.get()->camera.y / 2);
	if (IsShowing(GuiClass::PAUSE)) Engine::GetInstance().render.get()->DrawTexture(OptionsBook, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 185, -Engine::GetInstance().render.get()->camera.y / 2 + 20);

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
		break;
	}
}

void UI::Remove(GuiClass gui, int id) {
	switch (gui)
	{
	case GuiClass::PAUSE:
		pauseMenu.erase(pauseMenu.begin() + id);
		break;
	case GuiClass::TPBONFIRE:
		tpMenu.erase(tpMenu.begin() + id);
		break;
	case GuiClass::MAIN_MENU:
		mainMenu.erase(mainMenu.begin() + id);
		break;
	case GuiClass::SETTINGS:
		settings.erase(settings.begin() + id);
		break;
	}
}

void UI::Show(GuiClass gui, bool show) {

	switch (gui) {
	case GuiClass::PAUSE:
		for (auto button : pauseMenu) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		pauseMenuActive = true;
		break;
	case GuiClass::TPBONFIRE:
		for (auto button : tpMenu) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		tpMenuActive = true;
		break;
	case GuiClass::MAIN_MENU:
		for (auto button : mainMenu) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		mainMenuActive = true;
		break;
	case GuiClass::SETTINGS:
		for (auto button : settings) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		settingsActive = true;
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

bool UI::IsShowing(GuiClass gui) {
	switch (gui) {
	case GuiClass::PAUSE:
		for (auto button : pauseMenu) {
			return button->IsShowing();
		}
		break;
	case GuiClass::TPBONFIRE:
		for (auto button : tpMenu) {
			return button->IsShowing();
		}
		break;
	case GuiClass::MAIN_MENU:
		for (auto button : mainMenu) {
			return button->IsShowing();
		}
		break;
	case GuiClass::SETTINGS:
		for (auto button : settings) {
			return button->IsShowing();
		}
		break;
	}
}