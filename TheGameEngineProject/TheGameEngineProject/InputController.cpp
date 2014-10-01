#include "StdAfx.h"
#include "InputController.h"

CInputController::CInputController(CSDL_Setup* p_SDL_Setup, int *p_mouseX, int *p_mouseY, float* p_camX, float* p_camY, int p_screenW, int p_screenH, CEnvironment* p_environment)
{
	camX = p_camX;
	camY = p_camY;

	holdDeltaX = 0;
	holdDeltaY = 0;

	screenW = p_screenW;
	screenH = p_screenH;

	clicked = false;
	userInteractionAllowed = true;

	iPhoneHasAlreadyTapped = false;

	environment = p_environment;

	csdl_setup = p_SDL_Setup;
	mouseX = p_mouseX;
	mouseY = p_mouseY;

	timestamp = SDL_GetTicks();
}

CInputController::~CInputController(void)
{
}

void CInputController::updateInGameControls()
{
	customPoint mousePos;
	mousePos.x = *mouseX;
	mousePos.y = *mouseY;
	iPhoneHasAlreadyTapped = false;
	while (SDL_PollEvent(csdl_setup->getMainEvent()))
	{
		switch(csdl_setup->getMainEvent()->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (csdl_setup->getMainEvent()->button.button == SDL_BUTTON_LEFT && !clicked)
			{
				//environment->setButtonElementsDefault();
				mousePos.x = csdl_setup->getMainEvent()->button.x;
				mousePos.y = csdl_setup->getMainEvent()->button.y;

				environment->selectedStateToggle(mousePos);
				environment->HUDButtonToggle(mousePos);
				//printf("Mouse position: %d, %d\n", mousePos.x, mousePos.y);
				clicked = true;
				selectMouseX = mousePos.x;
				selectMouseY = mousePos.y;
				//if (environment->hitTestMenu(mousePos))
			}
			break;
		case SDL_MOUSEMOTION:
			if (clicked)
			{
				environment->setButtonElementsDefault();
				environment->selectedStateToggle(mousePos);
				environment->HUDButtonToggle(mousePos);
				//If finger is inside the menu, then don't move the camera!
				if (!environment->hitTestMenu(mousePos) && withinGameWorld(mousePos.x, mousePos.y))
					moveCamera();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			environment->setButtonElementsDefault();
			clicked = false;
			if (!iPhoneHasAlreadyTapped)
			{
				if (!environment->getCombatState()) //User interaction disabled when the game is in combat state
				{
					//We give the user some leeway when selecting; no need to be too precise!
					if (environment->hitTestButton(mousePos))
						environment->HUDButtonAction(mousePos);
					else if (tapSelected(*mouseX, *mouseY) && !environment->hitTestMenu(mousePos))
						environment->selected(*camX, *camY, *mouseX, *mouseY, posX, posY);
					else if (environment->hitTestMenu(mousePos))
						environment->selectedAction(*mouseX, *mouseY);
				}
				//If the combat animation is occuring and the user taps the screen
				else if (environment->getCombatState() && tapSelected(*mouseX, *mouseY))
				{

				}
				iPhoneHasAlreadyTapped = true;
			}
			//environment->HUDButtonAction(mousePos);
			//printf("Unclicked\n");
			break;
		default:
			break;
		}
	}
}

bool CInputController::withinGameWorld(int mouseX, int mouseY)
{
	if (mouseY >= screenH - ONSCREEN_MENU_HEIGHT) //checking to see if it's within the onscreen menu
		return false;
	return true;
}

bool CInputController::tapSelected(int mouseX, int mouseY)
{
	if (!withinGameWorld(mouseX, mouseY))
		return false;
	else if (abs(mouseX - selectMouseX) < 5 && abs(mouseY - selectMouseY) < 5)
		return true;
	return false;
}

void CInputController::moveCamera()
{
	holdDeltaX = -(csdl_setup->getMainEvent()->motion.xrel);
	holdDeltaY = -(csdl_setup->getMainEvent()->motion.yrel);
	//printf("%f AND %f\n", holdX, holdY);
	if (*camY + holdDeltaY >= 0 && *camY + screenH + holdDeltaY <= environment->getPixelHeight())
		*camY += holdDeltaY;
	if (*camX + holdDeltaX >= 0 && *camX + screenW + holdDeltaX <= environment->getPixelWidth())
		*camX += holdDeltaX;
}

void CInputController::update()
{
	updateInGameControls();
}
