#include "StdAfx.h"
#include "MainMenu.h"
#include "MainExec.h"

CMainMenu::CMainMenu(CSDL_Setup* p_SDL_setup, CMainExec* p_mainExec, int *mouseX, int *mouseY)
{
	mainExec_ = p_mainExec;
	exit_ = false;
	csdl_setup = p_SDL_setup;
}


CMainMenu::~CMainMenu(void)
{
}

void CMainMenu::GameExec()
{
	printf("Successfully entered main menu!\n");
}
