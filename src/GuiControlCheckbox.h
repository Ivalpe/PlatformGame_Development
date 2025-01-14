#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlCheckbox : public GuiControl
{

public:

	GuiControlCheckbox(int id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlCheckbox();

	// Called each loop iteration
	bool Update(float dt);

	void SetTexture(SDL_Texture* off, SDL_Texture* on) {
		checkboxOff = off;
		checkboxOn = on;
		actualCheck = checkboxOff;
		activated = false;
	}

	bool IsActiveFullScreen() {
		return activated;
	}

private:

	SDL_Texture* checkboxOff;
	SDL_Texture* checkboxOn;
	SDL_Texture* actualCheck;
	Vector2D posHitbox;
	Vector2D posTexture;
	bool drawBasic = false;
	bool activated;
};