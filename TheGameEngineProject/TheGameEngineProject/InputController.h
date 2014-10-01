#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "SDL_Setup.h"
#include <math.h>
#include <vector>
#include "Environment.h"
using namespace std;

class CInputController
{
public:
	CInputController(CSDL_Setup* p_SDL_Setup, int *p_mouseX, int *p_mouseY, float* p_camX, float* p_camY, int p_screenW, int p_screenH, CEnvironment* p_environment);
	~CInputController(void);

	bool getClicked() { return clicked; };
	bool withinGameWorld(int mouseX, int mouseY);

	void setUserInteraction(bool toggle) { userInteractionAllowed = toggle; }

	void update();
private:
	void updateInGameControls();
	void moveCamera();
	bool tapSelected(int mouseX, int mouseY);

	bool clicked;
	bool unitSelected;
	bool userInteractionAllowed;

	bool iPhoneHasAlreadyTapped;

	int posX;
	int posY;

	float* camX;
	float* camY;

	float holdDeltaX;
	float holdDeltaY;

	int screenW;
	int screenH;

	unsigned int timestamp;

	CSDL_Setup* csdl_setup;

	int selectMouseX;
	int selectMouseY;

	int *mouseX;
	int *mouseY;

	CEnvironment* environment; 
	
	bool idle;
};
