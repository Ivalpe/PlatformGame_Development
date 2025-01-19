#include "GuiControl.h"
#include "GuiControlButton.h"
#include "UI.h"
#include "Engine.h"
#include "GuiControlCheckbox.h"

bool UI::Update(float dt) {
	if (IsShowing(GuiClass::MAIN_MENU)) Engine::GetInstance().render.get()->DrawTexture(TitleScreen, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 110, -Engine::GetInstance().render.get()->camera.y / 2);
	if (IsShowing(GuiClass::PAUSE) || IsShowing(GuiClass::SETTINGS)) Engine::GetInstance().render.get()->DrawTexture(OptionsBook, SDL_FLIP_NONE, -Engine::GetInstance().render.get()->camera.x / 2 + 185, -Engine::GetInstance().render.get()->camera.y / 2 + 20);

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

bool UI::GetFullscreen() {
	GuiControlCheckbox* ch = (GuiControlCheckbox*)settings[2];
	return ch->IsActiveFullScreen();
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
		break;
	case GuiClass::TPBONFIRE:
		for (auto button : tpMenu) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		break;
	case GuiClass::MAIN_MENU:
		for (auto button : mainMenu) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		break;
	case GuiClass::SETTINGS:
		for (auto button : settings) {
			if (show) button->ShowOn();
			else button->ShowOff();
		}
		break;
	}
}

void UI::Active(GuiClass gui, int id) {

	switch (gui) {
	case GuiClass::PAUSE:
		pauseMenu[id - 1]->Enable();
		break;
	case GuiClass::TPBONFIRE:
		tpMenu[id - 1]->Enable();
		break;
	case GuiClass::MAIN_MENU:
		mainMenu[id - 1]->Enable();
		break;
	case GuiClass::SETTINGS:
		settings[id - 1]->Enable();
		break;
	}
}

void UI::Disable(GuiClass gui, int id) {
	switch (gui) {
	case GuiClass::PAUSE:
		pauseMenu[id - 1]->Disable();
		break;
	case GuiClass::TPBONFIRE:
		tpMenu[id - 1]->Disable();
		break;
	case GuiClass::MAIN_MENU:
		mainMenu[id - 1]->Disable();
		break;
	case GuiClass::SETTINGS:
		settings[id - 1]->Disable();
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

bool UI::CleanUp()
{
	tpMenu.clear();
	pauseMenu.clear();
	mainMenu.clear();
	settings.clear();
	SDL_DestroyTexture(OptionsBook);
	SDL_DestroyTexture(TitleScreen);

	return true;
}