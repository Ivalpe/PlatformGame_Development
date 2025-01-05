#include "GuiControlSlider.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "Log.h"

GuiControlSlider::GuiControlSlider(int id, SDL_Rect bounds) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	posTexture.setY(bounds.y);
	posHitbox.setY(bounds.y);
	posButton = 0;
	drawBasic = false;
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		posHitbox.setX(bounds.x);
		posTexture.setX(-(Engine::GetInstance().render.get()->camera.x / 2) + bounds.x);
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
		
		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > posHitbox.getX() && mousePos.getX() < (posHitbox.getX() + (bounds.w / 2)) && mousePos.getY() > posHitbox.getY() && mousePos.getY() < (posHitbox.getY() + bounds.h)) {

			state = GuiControlState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				posButton = mousePos.getX() - 8;
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
			Engine::GetInstance().render->DrawTexture(sliderBackground, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			Engine::GetInstance().render->DrawTexture(sliderMovement, SDL_FLIP_NONE, posButton, bounds.y);
			//Engine::GetInstance().render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			Engine::GetInstance().render->DrawTexture(sliderBackground, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			Engine::GetInstance().render->DrawTexture(sliderMovement, SDL_FLIP_NONE, posButton, bounds.y);
			//Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			Engine::GetInstance().render->DrawTexture(sliderBackground, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			Engine::GetInstance().render->DrawTexture(sliderMovement, SDL_FLIP_NONE, posButton, bounds.y);
			//Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			Engine::GetInstance().render->DrawTexture(sliderBackground, SDL_FLIP_NONE, posTexture.getX(), posTexture.getY());
			Engine::GetInstance().render->DrawTexture(sliderMovement, SDL_FLIP_NONE, posButton, bounds.y);
			//Engine::GetInstance().render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
			break;
		}
	}

	return false;
}

