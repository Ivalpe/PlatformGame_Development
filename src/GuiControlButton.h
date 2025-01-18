#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(int id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlButton();

	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	void SetTexture(SDL_Texture* normal, SDL_Texture* focused, SDL_Texture* pressed, SDL_Texture* disabled) {
		buttonNormal = normal;
		buttonFocused = focused;
		buttonPressed = pressed;
		buttonDisabled = disabled;
	}

private:

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonNormal;
	SDL_Texture* buttonFocused;
	SDL_Texture* buttonPressed;
	SDL_Texture* buttonDisabled;
	Vector2D posHitbox;
	Vector2D posTexture;
	int button_clickSFX;
	bool pressedSoundPlayed;
};

#pragma once