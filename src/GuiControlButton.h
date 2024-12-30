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

	void Disable();
	void Enable();
	bool IsActive() {
		return active;
	}
	void ActiveButton() {
		active = true;
	}

private:

	bool canClick = true;
	bool drawBasic = false;
	bool active = false;
};

#pragma once