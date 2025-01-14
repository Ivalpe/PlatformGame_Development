#include "GuiControlCheckbox.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "Log.h"

GuiControlCheckbox::GuiControlCheckbox(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	posTexture.setY(bounds.y);
	posHitbox.setY(bounds.y);
	drawBasic = false;
}

GuiControlCheckbox::~GuiControlCheckbox()
{

}

bool GuiControlCheckbox::Update(float dt)
{

	if (show)
	{
		if (activated) actualCheck = checkboxOn;
		else actualCheck = checkboxOff;

		posHitbox.setX(bounds.x);
		posTexture.setX(-(Engine::GetInstance().render.get()->camera.x / 2) + bounds.x);
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > posHitbox.getX() && mousePos.getX() < (posHitbox.getX() + (bounds.w / 2)) && mousePos.getY() > posHitbox.getY() && mousePos.getY() < (posHitbox.getY() + bounds.h)) {

			state = GuiControlState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				activated = !activated;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L16: TODO 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			Engine::GetInstance().render->DrawTexture(actualCheck, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::NORMAL:
			Engine::GetInstance().render->DrawTexture(actualCheck, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::FOCUSED:
			Engine::GetInstance().render->DrawTexture(actualCheck, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::PRESSED:
			Engine::GetInstance().render->DrawTexture(actualCheck, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		}
		Engine::GetInstance().render->DrawText(text.c_str(), posHitbox.getX() * 2, (posHitbox.getY() * 2) - 48, 180, 60, { 0,0,0 });
	}



	return false;
}

