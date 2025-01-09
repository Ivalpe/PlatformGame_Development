#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(int id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);

	void SetTexture(SDL_Texture* normal, SDL_Texture* focused, SDL_Texture* pressed) {
		buttonNormal = normal;
		buttonFocused = focused;
		buttonPressed = pressed;
	}

private:

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonNormal;
	SDL_Texture* buttonFocused;
	SDL_Texture* buttonPressed;
	Vector2D posHitbox;
	Vector2D posTexture;
};

#pragma once