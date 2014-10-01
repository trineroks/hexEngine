#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "SDL_Setup.h"
#include "GameState.h"

class CMainExec;

class CMainMenu : public CGameState
{
public:
	CMainMenu(CSDL_Setup* p_SDL_setup, CMainExec* p_mainExec, int *mouseX, int *mouseY);
	~CMainMenu(void);
	void GameExec();
private:

	CSDL_Setup *csdl_setup;
	bool exit_;
	CMainExec* mainExec_;
};

