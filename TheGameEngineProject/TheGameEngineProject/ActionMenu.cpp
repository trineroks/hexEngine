#include "StdAfx.h"
#include "ActionMenu.h"

CActionMenu::CActionMenu(CSDL_Setup *p_csdl_setup, SDL_Rect assetsPortion, int posX, int posY, int width)
{
	csdl_setup_ = p_csdl_setup;
	//buttonElements_ = new CGUI[numOfElements];
	actionMenuAssets_ = p_csdl_setup->getButtons();
	elementIndex_ = 0;

	position_.x = posX;
	position_.y = posY;

	labelFont_ = p_csdl_setup->getMenuLabel();
	labelOutline_ = p_csdl_setup->getMenuLabelOutline();

	//This should be for the buttons ONLY. assetsPortion_ should point to a box containing the default/clicked
	//state buttons, NOT THE HEADER OR THE FOOTER
	assetsPortion_.x = assetsPortion.x;
	assetsPortion_.y = assetsPortion.y;
	assetsPortion_.w = assetsPortion.w;
	assetsPortion_.h = assetsPortion.h;

	//The starting location for the first button
	rect_.x = posX;
	rect_.y = posY + ACTION_HEADER_HEIGHT;
	rect_.w = width;
	rect_.h = ACTION_BUTTON_HEIGHT;

	SDL_Rect portion; //On the MenuAssets sheet, this is where the header is located (in pixels)
	portion.x = 0;
	portion.y = 0;
	portion.h = 50;
	portion.w = 200;
	header_ = new CSprite(csdl_setup_->getRenderer(), actionMenuAssets_, width, ACTION_HEADER_HEIGHT, posX, posY, portion, 0, 0);

	//On the MenuAssets sheet, this is where the footer is located (in pixels)
	portion.x = 0;
	portion.y = 170;
	portion.h = 6;
	portion.w = 200;
	footer_ = new CSprite(csdl_setup_->getRenderer(), actionMenuAssets_, width, ACTION_FOOTER_HEIGHT, posX, 
						  posY + ACTION_HEADER_HEIGHT, portion, 0, 0);

	actionMenuBox_.x = posX;
	actionMenuBox_.y = posY;
	actionMenuBox_.w = width;
	//For both the header and the footer, we want to keep the height constant. Width is subject to change, but the height of
	//these elements should remain the same. We also place the footer after all the button elements. 
}

void CActionMenu::addElement(char p_string[], int actionToDo)
{
	int sheetParamsX = 1;
	int sheetParamsY = 2;
	elements_.push_back(new CGUI(csdl_setup_, rect_.x, rect_.y + (ACTION_BUTTON_HEIGHT * elementIndex_), rect_.w, rect_.h, assetsPortion_, sheetParamsX, sheetParamsY, p_string, actionToDo));
	elementIndex_++;
	updateActionMenuDimensions();
}

CActionMenu::~CActionMenu(void)
{
	delete header_;
	delete footer_;
	for (vector<CGUI*>::iterator i = elements_.begin(); i != elements_.end(); i++)
	{
		delete (*i);
		(*i) = NULL;
	}
	elements_.clear();
	csdl_setup_ = NULL;
	actionMenuAssets_ = NULL;
	header_ = NULL;
	footer_ = NULL;
	labelFont_ = NULL;
	labelOutline_ = NULL;
}

void CActionMenu::draw()
{
	header_->drawSteady();
	for (vector<CGUI*>::iterator i = elements_.begin(); i != elements_.end(); i++)
		(*i)->draw();
	footer_->drawSteady();
}

bool CActionMenu::mouseOverMenu(customPoint mouse)
{
	return !(mouse.x < actionMenuBox_.x || mouse.x > actionMenuBox_.x + actionMenuBox_.w || mouse.y < actionMenuBox_.y || mouse.y > actionMenuBox_.y + actionMenuBox_.h);
}

void CActionMenu::toggleButtonState(customPoint mouse)
{
	for (vector<CGUI*>::iterator i = elements_.begin(); i != elements_.end(); i++)
	{
		(*i)->stateDefault();
		if ((*i)->selected(mouse))
			(*i)->stateSelected();
	}
}

void CActionMenu::setToDefaultState()
{
	for (vector<CGUI*>::iterator i = elements_.begin(); i != elements_.end(); i++)
		(*i)->stateDefault();
}

int CActionMenu::getButtonPressed(customPoint mouse)
{
	int hold = 99;
	for (vector<CGUI*>::iterator i = elements_.begin(); i != elements_.end(); i++)
	{
		if ((*i)->selected(mouse))
		{
			hold = (*i)->getActionToDo();
		}
	}
	return hold;
}

void CActionMenu::updateActionMenuDimensions()
{
	actionMenuBox_.h = (ACTION_BUTTON_HEIGHT * elementIndex_) + ACTION_HEADER_HEIGHT + ACTION_FOOTER_HEIGHT;
	footer_->setY(position_.y + ACTION_HEADER_HEIGHT + (ACTION_BUTTON_HEIGHT * elementIndex_));
}