#include "StdAfx.h"
#include "GUI.h"
//THIS SHOULD'VE BEEN RENAMED BUTTON

CGUI::CGUI(void)
{
}

CGUI::CGUI(CSDL_Setup* csdl_setup, int p_x, int p_y, 
	       int p_w, int p_h, SDL_Rect p_Portion, int sheetParamX, int sheetParamY, char p_string[], int actionToDo)
		   : CSprite(csdl_setup->getRenderer(), csdl_setup->getButtons(), p_w, p_h, p_x, p_y, p_Portion, sheetParamX, sheetParamY)
{
	//setSheetParams(sheetParamX, sheetParamY);
	//stateDefault();
	actionToDo_ = actionToDo;
	buttonArea_.x = p_x;
	buttonArea_.y = p_y;
	buttonArea_.w = p_w;
	buttonArea_.h = p_h;

	buttonAssetsHorizontal_ = false;
	//setSheetParams(sheetParamX, sheetParamY);
	stateDefault();

	label_ = new CLabel(csdl_setup->getMenuLabel(), csdl_setup->getMenuLabelOutline(), csdl_setup->getRenderer());
	label_->setStringLabel(p_string);
	//label_->setPosition(500, 200);
	label_->setPosition(p_x + (p_w - label_->getWidth())/2, p_y + (p_h - label_->getHeight())/2); //Attempting to center text in box
}

CGUI::~CGUI(void)
{
	delete label_;
	label_ = NULL;
}

void CGUI::initialize(CSDL_Setup* csdl_setup, int p_x, int p_y, 
					  int p_w, int p_h, SDL_Rect p_Portion, int sheetParamX, int sheetParamY, char p_string[], int actionToDo)
{
	actionToDo_ = actionToDo;

	buttonAssetsHorizontal_ = false; //When we toggle between both button states, we want to make sure we crop in the corresponding direction
	//When there's 2 images laid side by side but only one image vertically on the button sprite sheet, we can deduce that the button assets are horizontal
	if (sheetParamX > sheetParamY)
		buttonAssetsHorizontal_ = true;

	buttonArea_.x = p_x;
	buttonArea_.y = p_y;
	buttonArea_.w = p_w;
	buttonArea_.h = p_h;

	initializeSprite(csdl_setup->getRenderer(), csdl_setup->getButtons(), p_w, p_h, p_x, p_y, p_Portion, sheetParamX, sheetParamY);
		
	//setSheetParams(sheetParamX, sheetParamY);
	stateDefault();
	label_ = new CLabel(csdl_setup->getMenuLabel(), csdl_setup->getMenuLabelOutline(), csdl_setup->getRenderer());
	label_->setStringLabel(p_string);
	label_->setPosition(p_x + (p_w - label_->getWidth())/2, p_y + (p_h - label_->getHeight())/2); //Attempting to center text in box
}

void CGUI::setActionToDo(int actionToDo)
{
	actionToDo_ = actionToDo;
}

void CGUI::setLabel(char p_string[])
{
	label_->setStringLabel(p_string);
}

void CGUI::stateDefault()
{
	cropImage(0, 0);
}

void CGUI::stateSelected()
{
	if (buttonAssetsHorizontal_)
		cropImage(1, 0);
	else
		cropImage(0, 1);
}

void CGUI::draw()
{
	drawSteady();
	label_->drawSteadyLabel();
}

bool CGUI::selected(customPoint mouse)
{
	return !(mouse.x < buttonArea_.x || mouse.x > buttonArea_.x + buttonArea_.w || mouse.y < buttonArea_.y || mouse.y > buttonArea_.y + buttonArea_.h);
}


