#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(int id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);

	void SetTexture(SDL_Texture* background, SDL_Texture* movement) {
		sliderBackground = background;
		sliderMovement = movement;
	}

private:

	SDL_Texture* sliderBackground;
	SDL_Texture* sliderMovement;
	int posButton;
	Vector2D posHitbox;
	Vector2D posTexture;
	bool canClick = true;
	bool drawBasic = false;
};