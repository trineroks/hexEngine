#include "StdAfx.h"
#include "Main.h"
#include "MainExec.h"

CMain::CMain(CSDL_Setup *p_setup, int *p_board, customPoint p_dimensions, int *mouseX, int *mouseY, 
	bool teamGame, int numOfPlayers, CPlayer *p_players, CMainExec *p_mainExec)
	: CGameState()
{
	camX = 0; camY = 0;
	exit_ = false;
	mainExec_ = p_mainExec;

	int board[1024] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0,
		0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 3, 0,
		0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0,
		0, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0,
		0, 1, 2, 1, 1, 2, 1, 1, 3, 3, 1, 2, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 0,
		0, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 0,
		0, 1, 2, 1, 3, 3, 1, 1, 1, 1, 1, 2, 1, 0,
		0, 1, 1, 3, 3, 1, 1, 2, 1, 1, 1, 2, 2, 0,
		0, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0,
		0, 3, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0,
		0, 3, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	//mouseX = 0; mouseY = 0;
	zone_level = new CEnvironment(board, numOfPlayers, p_players, p_dimensions.x, p_dimensions.y, p_setup, &exit_);
	controller = new CInputController(p_setup, mouseX, mouseY, &camX, &camY, p_setup->getWidth(), p_setup->getHeight(), zone_level);
}


CMain::~CMain(void)
{
	delete controller;
	delete zone_level;
	mainExec_ = NULL;
	controller = NULL;
	zone_level = NULL;
	exit_ = NULL;
}

void CMain::GameExec()
{
	if (exit_)
		mainExec_->setNextState(STATE_MENU);
	controller->update();
	zone_level->update(camX, camY);
}
