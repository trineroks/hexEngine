#pragma once
#include "SDL_Setup.h"
#include "Player.h"
#include "Sprite.h"
#include "InputController.h"
#include "Environment.h"
#include "GameState.h"

class CMainExec;

class CMain : public CGameState
{
public:
	CMain(CSDL_Setup *p_setup, int *p_board, customPoint p_dimensions, int *mouseX, int *mouseY, 
		  bool teamGame, int numOfPlayers, CPlayer *p_players, CMainExec *p_mainExec);
	~CMain(void);
	void GameExec();

private:
	float camX, camY;

	CInputController* controller;
	CEnvironment* zone_level;

	bool exit_;
	CMainExec *mainExec_;

	//int mouseX, mouseY;
};

