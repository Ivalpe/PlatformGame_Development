#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(int id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	posTexture.setY(bounds.y / 2);
	posHitbox.setY(bounds.y);
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (show)
	{
		posHitbox.setX(bounds.x);
		posTexture.setX(-(Engine::GetInstance().render.get()->camera.x / 2) + bounds.x / 2);
		if (state != GuiControlState::DISABLED) {

			Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

			//If the position of the mouse if inside the bounds of the button 
			if (mousePos.getX() > posHitbox.getX() / 2 && mousePos.getX() < (posHitbox.getX() + bounds.w) / 2 && mousePos.getY() > posHitbox.getY() / 2 && mousePos.getY() < (posHitbox.getY() + bounds.h) / 2) {

				state = GuiControlState::FOCUSED;

				if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
					state = GuiControlState::PRESSED;
				}

				if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
					NotifyObserver();
				}
			}
			else {
				state = GuiControlState::NORMAL;
			}
		}

		switch (state)
		{
		case GuiControlState::DISABLED:
			Engine::GetInstance().render->DrawTexture(buttonDisabled, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::NORMAL:
			Engine::GetInstance().render->DrawTexture(buttonNormal, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::FOCUSED:
			Engine::GetInstance().render->DrawTexture(buttonFocused, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		case GuiControlState::PRESSED:
			Engine::GetInstance().render->DrawTexture(buttonPressed, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			break;
		}

		std::string name = "   ";
		name = name + text.c_str();
		Engine::GetInstance().render->DrawText(name.c_str(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

