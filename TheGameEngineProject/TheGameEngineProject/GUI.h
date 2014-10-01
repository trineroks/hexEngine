#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Label.h"
#include "SDL_Setup.h"

class CGUI : public CSprite
{
public:
	CGUI();
	CGUI(CSDL_Setup* csdl_setup, int p_x, int p_y, int p_w, int p_h, SDL_Rect p_Portion, int sheetParamX, int sheetParamY, char p_string[], int actionToDo);
	~CGUI(void);

	void initialize(CSDL_Setup* p_setup, int p_x, int p_y, 
					int p_w, int p_h, SDL_Rect p_Portion, int sheetParamX, int sheetParamY, char p_string[], int actionToDo);
	void draw();

	void stateSelected();
	void stateDefault();

	int getActionToDo() { return actionToDo_; }
	void setActionToDo(int actionToDo);

	void setLabel(char p_string[]);

	bool selected(customPoint mouse);

	int getX() { return buttonArea_.x; }
	int getY() { return buttonArea_.y; }

private:
	SDL_Rect buttonArea_;
	bool buttonAssetsHorizontal_;
	CLabel *label_;
	int actionToDo_;
};

