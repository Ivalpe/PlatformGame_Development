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

	void SetTexture(SDL_Texture* normal) {
		buttonNormal = normal;
	}

private:

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonNormal;
	Vector2D posHitbox;
	Vector2D posTexture;
};

#pragma once